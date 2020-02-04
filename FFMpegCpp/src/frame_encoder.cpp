#include "encoding/frame_encoder.h"

#include "common/error_codes.h"

namespace codec {
namespace encoder {

FrameEncoder::FrameEncoder(
    const int fps, 
    const int width, 
    const int height,
    const int max_packets)
    : width_(width),
      height_(height),
      packets_(max_packets)
{
  encoder_context_ = std::make_unique<EncoderContext>(fps, width, height);
  
  encoder_frame_ = std::make_unique<EncoderFrame>(
      encoder_context_->GetPixelFormat(), width, height);
  
  pixel_converter_ = std::make_unique<PixelConverter>(
      SRC_PIXEL_FORMAT, encoder_context_->GetPixelFormat(), width, height);
}

int FrameEncoder::EncodeFrame(
    uint8_t* raw_data, 
    int raw_length,
    uint8_t** encoded_data,
    int* encoded_length)
{
  if (!raw_data || !encoded_data || !encoded_length)
    return ErrorCode::INVALID_PARAMETER;
  
  int err_code = encoder_frame_->EnsureWritable();
  if (err_code < 0) return err_code;
  
  AVFrame frame = detail::FrameFromRawData(
      raw_data, raw_length, SRC_PIXEL_FORMAT, height_);
  encoder_frame_->SetData(&frame, *pixel_converter_);
  encoder_frame_->IncrementFrameCount();
  
  err_code = avcodec_send_frame(
      encoder_context_->GetContextPtr(), encoder_frame_->GetFramePtr());
  if (err_code != 0)
    return err_code;

  int num_received = 0;
  for (Packet &packet : packets_) {
    if (num_received == packets_.size()) break;

    err_code = avcodec_receive_packet(
        encoder_context_->GetContextPtr(),
        packet.GetPacketPtr());

    if (err_code == 0) {
      ++num_received;
      encoded_length[num_received - 1] = packet.Size();
      encoded_data[num_received - 1] = packet.Data();
    } else if (err_code == AVERROR(EAGAIN) || err_code == AVERROR(AVERROR_EOF)) {
      break;
    } else {
      return err_code;
    }
  }
  return num_received;
}

namespace detail {

AVFrame FrameFromRawData(
    uint8_t* raw_data, 
    int raw_length,
    AVPixelFormat format, 
    int height)
{
  AVFrame frame;
  frame.data[0] = raw_data;
  frame.linesize[0] = raw_length / height;
  frame.height = height;
  frame.format = format;
  return frame;
}

}  // namespace detail

}  // encoder
}  // namespace codec

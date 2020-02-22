#include "encoding/frame_encoder.h"

#include "common/codec_exception.h"

namespace codec {
namespace encoder {

FrameEncoder::FrameEncoder(const int fps, const int width, const int height)
    : height_(height)
{
  encoder_context_ = std::make_unique<EncoderContext>(fps, width, height);

  encoder_frame_ = std::make_unique<EncoderFrame>(
      encoder_context_->GetPixelFormat(), width, height);

  pixel_converter_ = std::make_unique<PixelConverter>(
      SRC_PIXEL_FORMAT, encoder_context_->GetPixelFormat(), width, height);
}

std::vector<std::unique_ptr<Packet>> FrameEncoder::EncodeFrame(
    uint8_t *const raw_data, const int raw_length)
{
  if (!raw_data)
    throw std::runtime_error("Raw Data supplied to encoder is null!");

  if (raw_length <= 0)
    throw std::runtime_error("Raw data length supplied to encoder is invalid");

  int err_code = encoder_frame_->EnsureWritable();
  if (err_code < 0)
      throw CodecException(
          err_code,
          "Error in FFMpeg library: Frame is not writable");

  AVFrame frame = detail::FrameFromRawData(
      raw_data, raw_length, SRC_PIXEL_FORMAT, height_);

  encoder_frame_->SetData(&frame, *pixel_converter_);
  encoder_frame_->IncrementFrameCount();

  err_code = avcodec_send_frame(
      encoder_context_->GetContextPtr(), encoder_frame_->GetFramePtr());
  if (err_code < 0)
    throw CodecException(
        err_code,
        "Error in ffmpeg library: Could not send frame to encoder");

  std::vector<std::unique_ptr<Packet>> packets;
  while (true) {
    auto packet = std::make_unique<Packet>();
    err_code = avcodec_receive_packet(
        encoder_context_->GetContextPtr(),
        packet->GetPacketPtr());

    if (err_code >= 0) {
      packets.emplace_back(std::move(packet));
    } else if (err_code == AVERROR(EAGAIN) || err_code == AVERROR(AVERROR_EOF)) {
      break;
    } else {
      throw CodecException(
          err_code,
          "Error in FFMpeg library: Error receiving packets from encoder");
    }
  }

  return packets;
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

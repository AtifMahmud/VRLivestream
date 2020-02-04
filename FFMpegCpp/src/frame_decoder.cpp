#include "decoding/frame_decoder.h"

#include <string>

#include "common/packet.h"
#include "common/error_codes.h"

namespace codec {
namespace decoder {

int FrameDecoder::DecodeFrame(
    uint8_t *const raw_data, 
    const int raw_length)
{
  if (buffers_.size() == 0)
    return codec::ErrorCode::INVALID_PARAMETER;
    
  Packet packet(raw_data, raw_length);
  int err_code = avcodec_send_packet( 
      decoder_context_.GetContext(), 
      packet.GetPacketPtr());

  if (err_code != 0)
    return err_code;

  int num_received = 0;
  for (auto &buffer : buffers_) {
    if (num_received == buffers_.size()) break;

    err_code = avcodec_receive_frame(
        decoder_context_.GetContext(), 
        decoder_frame_.GetFramePtr());
    
    if (err_code == 0) {
      if (!pixel_converter_)
        pixel_converter_ =  std::make_unique<PixelConverter>(
            static_cast<AVPixelFormat>(decoder_frame_.PixelFormat()),
            OUT_PIX_FMT,
            decoder_frame_.Width(),
            decoder_frame_.Height());

      pixel_converter_->ConvertPixels(
          decoder_frame_.GetFramePtr(), 
          buffer->GetData(), 
          buffer->GetLineSize());

      ++num_received;
    } else if (err_code == AVERROR(EAGAIN) || err_code == AVERROR(AVERROR_EOF)) {
      break;
    } else {
      return err_code;
    }
  }
  
  return num_received;
}

int FrameDecoder::GetBufferSize(int image_width, int image_height) const
{
  return DecodedFrameBuffer::GetRequiredBufferSize(
      image_width, image_height, OUT_PIX_FMT);
}

int FrameDecoder::AddBuffer(
    uint8_t* buffer, 
    const int width, 
    const int height)
{
  buffers_.emplace_back(std::make_unique<DecodedFrameBuffer>(
      width, height, OUT_PIX_FMT));
  return buffers_.at(buffers_.size() - 1)->SetBuffer(buffer);
}

}  // namespace decoder
}  // namespace codec

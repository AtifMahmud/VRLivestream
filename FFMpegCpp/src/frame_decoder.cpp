/**
 * @file frame_decoder.cpp
 * @brief Implementation of the FrameDecoder class
 */

#include "decoding/frame_decoder.h"

#include "common/codec_exception.h"

namespace codec {
namespace decoder {

FrameDecoder::FrameDecoder(
     const AVPixelFormat out_pxl_fmt, const AVCodecID codec_id)
    : out_pxl_fmt_(out_pxl_fmt)
{
  decoder_context_ = std::make_unique<DecoderContext>(codec_id);
}

void FrameDecoder::DecodeFrame(Packet *const packet)
{
  if (!packet)
    throw std::runtime_error(
        "Error: Null components supplied to DecodeFrame function");

  int err_code = avcodec_send_packet(
      decoder_context_->GetContext(),
      packet->GetPacketPtr());

  if (err_code != 0)
    throw CodecException(err_code, "Could not send packet to decoder");
}

bool FrameDecoder::ReceiveFrames(DecodedFrameBuffer *const buffer)
{
  int err_code = avcodec_receive_frame(
      decoder_context_->GetContext(),
      decoder_frame_.GetFramePtr());

  if (err_code >= 0) {
    if (!pixel_converter_)
      pixel_converter_ =  std::make_unique<PixelConverter>(
          static_cast<AVPixelFormat>(decoder_frame_.PixelFormat()),
          out_pxl_fmt_,
          decoder_frame_.Width(),
          decoder_frame_.Height());

    pixel_converter_->ConvertPixels(
        decoder_frame_.GetFramePtr(),
        buffer->GetData(),
        buffer->GetLineSize());

    return true;
  }
  else if (err_code == AVERROR(EAGAIN) || err_code == AVERROR(AVERROR_EOF)) {
    return false;
  } else {
    throw CodecException(err_code, "Failure decoding packet");
  }
}

}  // namespace decoder
}  // namespace codec

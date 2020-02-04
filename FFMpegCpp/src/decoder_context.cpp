#include "decoding/decoder_context.h"

#include <string>

#include "common/codec_exception.h"
#include "common/error_codes.h"

extern "C" {
#include <libavformat/avformat.h>
}

namespace codec {
namespace decoder {

DecoderContext::DecoderContext()
{
  av_register_all();
  AVCodecID codec_id = AVCodecID::AV_CODEC_ID_H264;
  codec_ptr_ = avcodec_find_decoder(codec_id);
  if (!codec_ptr_) 
    throw CodecException(
        ErrorCode::GENERIC_CODEC_EXCEPTION, 
        "Decoder Context: Codec not found.");

  codec_context_ptr_ = avcodec_alloc_context3(codec_ptr_);
  if (!codec_context_ptr_) {
    throw CodecException(
        ErrorCode::GENERIC_CODEC_EXCEPTION, 
        "Decoder Context:: Could not allocate codec context.");
  }

  int success = avcodec_open2(codec_context_ptr_, codec_ptr_, nullptr);
  if (success < 0) {
    throw CodecException(
        success, 
        "Decoder Context: Could not open codec");
  }
}

DecoderContext::~DecoderContext()
{
  avcodec_close(codec_context_ptr_);
  av_free(codec_context_ptr_);
  av_free(codec_ptr_);
}

}  // namespace codec
}  // namespace decoder

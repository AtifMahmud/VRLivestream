#include "decoding/decoder_context.h"

extern "C" {
#include <libavformat/avformat.h>
}

#include <stdexcept>
#include <string>

#include "common/codec_exception.h"

namespace codec {
namespace decoder {

DecoderContext::DecoderContext(const AVCodecID codec_id)
{
  codec_ptr_ = avcodec_find_decoder(codec_id);
  if (!codec_ptr_)
    throw std::runtime_error("Decoder Context: Codec not found.");

  codec_context_ptr_ = avcodec_alloc_context3(codec_ptr_);
  if (!codec_context_ptr_)
    throw std::runtime_error(
        "Decoder Context:: Could not allocate codec context.");

  int success = avcodec_open2(codec_context_ptr_, codec_ptr_, nullptr);
  if (success < 0)
    throw CodecException(success, "Decoder Context: Could not open codec");
}

DecoderContext::~DecoderContext()
{
  avcodec_close(codec_context_ptr_);
  av_free(codec_context_ptr_);
  av_free(codec_ptr_);
}

}  // namespace codec
}  // namespace decoder

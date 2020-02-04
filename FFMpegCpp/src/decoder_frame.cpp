#include "decoding/decoder_frame.h"

#include "common/codec_exception.h"
#include "common/error_codes.h"

namespace codec {
namespace decoder {

DecoderFrame::DecoderFrame()
{
  frame_ = av_frame_alloc();
  if (!frame_)
    throw CodecException(
        ErrorCode::FAILED_ALLOC, "Cannot allocate memory for decoder frame");
}

DecoderFrame::~DecoderFrame()
{
  av_frame_free(&frame_);
}

}  // namespace decoder
}  // namespace codec

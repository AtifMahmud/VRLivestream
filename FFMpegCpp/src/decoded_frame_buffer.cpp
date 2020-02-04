#include "decoding/decoded_frame_buffer.h"

extern "C" {
#include <libavutil/imgutils.h>
}

#include "common/codec_exception.h"
#include "common/error_codes.h"

namespace codec {
namespace decoder {

DecodedFrameBuffer::DecodedFrameBuffer(
    const int width, const int height, const AVPixelFormat pxl_fmt)
    : width_(width),
      height_(height),
      pxl_fmt_(pxl_fmt)
{
  frame_ = av_frame_alloc();
  if (!frame_)
    throw CodecException(
        ErrorCode::FAILED_ALLOC, 
        "Frame: Cannot allocate memory for decoder frame");
}

DecodedFrameBuffer::~DecodedFrameBuffer()
{
  av_frame_free(&frame_);
}

int DecodedFrameBuffer::GetRequiredBufferSize(
    const int width, const int height, const AVPixelFormat pxl_fmt)
{
  return av_image_get_buffer_size(pxl_fmt, width, height, 1);
}

int DecodedFrameBuffer::SetBuffer(uint8_t *const buffer)
{
  return av_image_fill_arrays(
      frame_->data, 
      frame_->linesize,
      buffer,
      pxl_fmt_,
      width_, 
      height_, 
      1);
}


}  // namespace decoder
}  // namespace codec

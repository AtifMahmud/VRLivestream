/**
 * @file decoded_frame_buffer.cpp
 * @brief Contains the implementation of the DecodedFrameBuffer class
 */

#include "decoding/decoded_frame_buffer.h"

extern "C" {
#include <libavutil/imgutils.h>
}

#include <stdexcept>

#include "common/codec_exception.h"

namespace codec {
namespace decoder {

DecodedFrameBuffer::DecodedFrameBuffer(
    const int width,
    const int height,
    const AVPixelFormat pxl_fmt,
    uint8_t *const buffer)
{
  frame_ = av_frame_alloc();
  if (!frame_)
    throw std::runtime_error("Error: Cannot allocate memory for decoder frame");

  int err = av_image_fill_arrays(
      frame_->data,
      frame_->linesize,
      buffer,
      pxl_fmt,
      width,
      height,
      1);

  if (err < 0)
    throw CodecException(err, "Cannot set frame data for decoder buffer");
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

}  // namespace decoder
}  // namespace codec

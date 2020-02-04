#include "common/pixel_converter.h"

#include "common/codec_exception.h"
#include "common/error_codes.h"

namespace codec {

PixelConverter::PixelConverter(
    const AVPixelFormat src_format, 
    const AVPixelFormat dest_format,
    const int width, 
    const int height)
    : src_format_(src_format),
      dest_format_(dest_format),
      height_(height)
{
  sws_context_ = sws_getContext(
      width, height, src_format_,
      width, height, dest_format_, 
      SWS_FAST_BILINEAR, 
      nullptr, nullptr, nullptr);

  if (!sws_context_)
    throw CodecException(
        ErrorCode::GENERIC_CODEC_EXCEPTION, 
        "Cannot create sws_context for pixel converter"); 
}

PixelConverter::~PixelConverter()
{
  if (sws_context_) sws_freeContext(sws_context_);
}

void PixelConverter::ConvertPixels(
    const AVFrame *const src_frame, 
    uint8_t* *const dest_data, 
    int *const dest_line_size) const
{
  sws_scale(
      sws_context_, 
      src_frame->data, 
      src_frame->linesize, 
      0, 
      height_, 
      dest_data, 
      dest_line_size);
}

}  // namespace codec

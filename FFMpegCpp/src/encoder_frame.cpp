/**
 * @file encoder_frame.cpp
 * @brief Implementation of the EncoderFrame class
 */

#include "encoding/encoder_frame.h"

#include <stdexcept>

#include "common/codec_exception.h"

namespace codec {
namespace encoder {

EncoderFrame::EncoderFrame(
    const AVPixelFormat pixel_format,
    const int width,
    const int height)
    : pixel_format_(pixel_format)
{
  frame_ = av_frame_alloc();
  if (!frame_)
    throw std::runtime_error("Cannot allocate memory for encoder frame");

  frame_->format = pixel_format;
  frame_->width = width;
  frame_->height = height;

  int success = av_frame_get_buffer(frame_, 0);
  if (success < 0)
    throw CodecException(
        success,
        "Cannot allocate memory for frame data");
}

EncoderFrame::~EncoderFrame()
{
  av_frame_free(&frame_);
}

int EncoderFrame::EnsureWritable()
{
  return av_frame_make_writable(frame_);
}

void EncoderFrame::IncrementFrameCount()
{
  frame_->pts = frame_count_;
  ++frame_count_;
}

void EncoderFrame::SetData(
    AVFrame *const src_frame, const PixelConverter &converter)
{
  converter.ConvertPixels(src_frame, frame_->data, frame_->linesize);
}

}  // encoder
}  // namespace codec

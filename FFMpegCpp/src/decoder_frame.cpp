/**
 * @file decoder_frame.cpp
 * @brief Implementation of the DecoderFrame class
 */

#include "decoding/decoder_frame.h"

#include <stdexcept>

#include "common/codec_exception.h"

namespace codec {
namespace decoder {

DecoderFrame::DecoderFrame()
{
  frame_ = av_frame_alloc();
  if (!frame_)
    throw std::runtime_error("Cannot allocate memory for decoder frame");
}

DecoderFrame::~DecoderFrame()
{
  av_frame_free(&frame_);
}

}  // namespace decoder
}  // namespace codec

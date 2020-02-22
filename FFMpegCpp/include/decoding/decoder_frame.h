/**
 * @file decoder_frame.h
 * @brief Holds the declaration of the DecoderFrame class
 */

#pragma once

extern "C" {
#include <libavcodec/avcodec.h>
}

namespace codec {
namespace decoder {

/**
 * @brief An RAII wrapper around the AVFrame class for decoding
 */
class DecoderFrame {
 public:
  /**
   * @brief Construct a new DecoderFrame
   *
   * @throw std::runtime_error If the underlying FFMpeg object could not be allocated
   */
  DecoderFrame();

  // Delete the copy and assignment operators
  DecoderFrame(const DecoderFrame&) = delete;
  DecoderFrame operator=(const DecoderFrame&) = delete;

  /**
   * @brief Destroy the DecoderFrame object
   */
  ~DecoderFrame();

  // Getter functions for the underlying FFMpeg data
  inline AVFrame* GetFramePtr() { return frame_; }
  inline uint8_t ** Data()      { return frame_->data; }
  inline int * LineSize()       { return frame_->linesize; }
  inline int Width() const      { return frame_->width; }
  inline int Height() const     { return frame_->height; }
  inline int DataSize() const   { return frame_->height * frame_->linesize[0];}

  inline AVPixelFormat PixelFormat() const {
    return static_cast<AVPixelFormat>(frame_->format);
  }

 private:
  AVFrame* frame_;
};

}  // namespace decoder
}  // namespace codec

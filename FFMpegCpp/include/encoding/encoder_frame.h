/**
 * @file encoder_frame.h
 * @brief Holds the declaration of the EncoderFrame class
 */

#pragma once

extern "C" {
#include <libavcodec/avcodec.h>
}

#include "common/pixel_converter.h"

namespace codec {
namespace encoder {

/**
 * @brief An RAII wrapper around the AVFrame object to be used for encoding
 */
class EncoderFrame {
 public:
  /**
   * @brief Construct a new EncoderFrame object
   *
   * @param pixel_format The pixel format of frames to encode
   * @param width The width of each image frame
   * @param height The height of each image frame
   *
   * @throw std::runtime_error If the underling FFMpeg pointer could not be allocated
   * @throw CodecException On an FFMpeg error
   */
  EncoderFrame(AVPixelFormat pixel_format, int width, int height);

  // Delete the copy constructor and the assignment operator
  EncoderFrame(const EncoderFrame&) = delete;
  EncoderFrame operator=(const EncoderFrame&) = delete;

  /**
   * @brief Destroy the EncoderFrame
   */
  ~EncoderFrame();

  /**
   * @brief Ensure that the frame is still writeable. Should be called before
   *        attempting to send data to the encoder
   *
   * @return int a negative ffmepeg error code if it is not writeable
   */
  int EnsureWritable();

  /**
   * @brief Increment the frame count. Should be called after every call to SetData
   */
  void IncrementFrameCount();

  /**
   * @brief Set the underlying image frame data
   *
   * @param src_frame The src frame
   * @param converter A convertor to convert the pixels into the appropiate pixel type
   */
  void SetData(AVFrame* src_frame, const PixelConverter &converter);

  /**
   * @brief Get the underlying FFMpeg pointer
   */
  inline AVFrame* GetFramePtr() { return frame_; }

 private:
  AVFrame* frame_;
  AVPixelFormat pixel_format_;
  int frame_count_;
};

}  // encoder
}  // namespace codec

/**
 * @file decoded_frame_buffer.h
 * @brief Holds the declaration of the DecodedFrameBuffer class
 */

#pragma once

extern "C" {
#include <libavcodec/avcodec.h>
}

namespace codec {
namespace decoder {

/**
 * @brief A class to wrap a single output buffer for a decoded image frame
 */
class DecodedFrameBuffer {
 public:
  /**
   * @brief Construct a new buffer
   *
   * @param width The width of the decoded images
   * @param height The height of the decoded images
   * @param pxl_fmt The pixel format for the decoded images
   * @param buffer A pointer to store the data in. It must be of the correct size
   *               The size can be found by calling GetRequiredBufferSize
   *
   * @throw std::runtime_error If the underlying FFMpeg object could not be allocated
   * @throw CodecException If the FFMpeg library fails to set the underlying object data
   */
  DecodedFrameBuffer(
      int width, int height, AVPixelFormat pxl_fmt, uint8_t* buffer);

  // Delete the copy constructor  and assignment operator
  DecodedFrameBuffer(const DecodedFrameBuffer&) = delete;
  DecodedFrameBuffer operator=(const DecodedFrameBuffer&) = delete;

  /**
   * @brief Destroy the Decoded Frame Buffer object
   */
  ~DecodedFrameBuffer();

  /**
   * @brief Get the underlying data pointer
   */
  inline uint8_t** GetData() { return frame_->data; }

  /**
   * @brief Get the underlying linesize pointer
   */
  inline int* GetLineSize()  { return frame_->linesize; }

  /**
   * @brief Get the required buffer size for an image frame with the supplied specs
   *
   * @param width The width of the decoded image frames
   * @param height The height of the decoded image frames
   * @param pxl_fmt The pixel format of the decoded image frames
   */
  static int GetRequiredBufferSize(
      int width, int height, AVPixelFormat pxl_fmt);

 private:
  AVFrame* frame_;
};

}  // namespace decoder
}  // namespace codec

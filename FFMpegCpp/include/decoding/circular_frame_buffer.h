/**
 * @file circular_frame_buffer.h
 * @brief Contains the declaration of the CircularFrameBuffer class
 */

#pragma once

extern "C" {
#include <libavcodec/avcodec.h>
}

#include <cstdint>
#include <memory>
#include <mutex>
#include <vector>

#include "decoding/decoded_frame_buffer.h"

namespace codec {
namespace decoder {

/**
 * @brief Holds a circular array of the decoding buffers.
 *
 * This class is used to hold multiple buffers that can hold decoded frame data
 * It keeps track of the frame last read by the client, as well as the the most recent
 * frame received from the server. Once all the buffers have been written to,
 * the class will loop back to overwrite the older buffers.
 *
 * There must be at least 3 buffers for this class to work. However, a larger
 * amount of buffers decreases the chance that the client will miss a frame due
 * to it being overwritten.
 */
class CircularFrameBuffer {
 public:
  /**
   * @brief Construct a new CircularFrameBuffer object
   */
  CircularFrameBuffer();

  /**
   * @brief Adds a buffer to the back of the circular buffer
   *
   * @param buffer The buffer to add
   * @param width The width of the image frames
   * @param height The height of the image frames
   * @param pxl_fmt The pixel format of the decoded images
   */
  void AddBuffer(uint8_t* buffer, int width, int height, AVPixelFormat pxl_fmt);

  /**
   * @brief Update and return the read buffer index
   *
   * This will lock this buffer from being written to until SetReadBuffer
   * is called again and a new read buffer is returned
   *
   * @throw std::runtime_error If there are not at least 3 buffers that have been added
   */
  int SetReadBuffer();

  /**
   * @brief Start a write to a buffer. If the write is successful, a call to
   *        WriteSuccess should be made. If it is not successful, StartWrite can
   *        be called again
   *
   * The buffer will be locked from reading until WriteSuccess is called
   *
   * @return DecodedFrameBuffer* A pointer to the buffer to write to
   */
  DecodedFrameBuffer* StartWrite();

  /**
   * @brief Indicate that the last write was successful. This will unlock this
   * buffer so that it can be read from. And will increment the the buffer index
   * to write to
   */
  void WriteSuccess();

 private:
  /**
   * @brief Increments the given buffer index, looping the index back to 0
   *        if it passes the end of the circular array

   * @param curr_index The current value of the index
   *
   * @return int The new value of the index
   */
  int IncrementBuffIndex(int curr_index);

  std::vector<std::unique_ptr<DecodedFrameBuffer>> buffers_;

  int write_buff_;
  int read_buff_;

  std::mutex mutex_;
};

}  // namespace decoder
}  // namespace codec

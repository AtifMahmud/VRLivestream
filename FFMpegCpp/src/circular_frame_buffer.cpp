/**
 * @file circular_frame_buffer.cpp
 * @brief Contains the implementation of the CircularFrameBuffer class
 */

#include "decoding/circular_frame_buffer.h"

namespace codec {
namespace decoder {

CircularFrameBuffer::CircularFrameBuffer()
    : write_buff_(0), read_buff_(-1)
{}

void CircularFrameBuffer::AddBuffer(
    uint8_t *const buffer,
    const int width,
    const int height,
    const AVPixelFormat pxl_fmt)
{
  std::lock_guard<std::mutex> lck(mutex_);

  auto buf = std::make_unique<DecodedFrameBuffer>(
      width, height, pxl_fmt, buffer);

  buffers_.emplace_back(std::move(buf));
}

int CircularFrameBuffer::SetReadBuffer()
{
  std::lock_guard<std::mutex> lck(mutex_);

  if (buffers_.size() < 3)
    throw std::runtime_error(
        "There must be at least 3 buffers for the decoder");

  // Have yet to write data
  if (read_buff_ < 0 && write_buff_ == 0)
    return -1;

  int new_index = IncrementBuffIndex(read_buff_);

  // No new data since last read, or the writer lapped the reader
  // Either way, cannot currently read any data
  if (new_index == write_buff_)
    return -1;

  read_buff_ = new_index;

  return read_buff_;
}

DecodedFrameBuffer* CircularFrameBuffer::StartWrite()
{
  std::lock_guard<std::mutex> lck(mutex_);

  if (buffers_.size() < 3)
    throw std::runtime_error(
        "There must be at least 3 buffers for the decoder");

  return buffers_[write_buff_].get();
}

void CircularFrameBuffer::WriteSuccess()
{
  std::lock_guard<std::mutex> lck(mutex_);
  write_buff_ = IncrementBuffIndex(write_buff_);
  if (write_buff_ == read_buff_)  // If we lapped the reader
    IncrementBuffIndex(write_buff_);
}

int CircularFrameBuffer::IncrementBuffIndex(const int curr_index)
{
  if (curr_index == static_cast<int>(buffers_.size()) - 1)
    return 0;
  else
    return curr_index + 1;
}

}  // namespace decoder
}  // namespace codec

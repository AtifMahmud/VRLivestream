/**
 * @file stream_spinner.h
 * @brief Holds the declaration of the StreamSpinner class
 */

#pragma once

#include <atomic>
#include <functional>
#include <memory>
#include <thread>

#include "common/packet.h"
#include "decoding/frame_decoder.h"
#include "decoding/circular_frame_buffer.h"

namespace codec {
namespace stream {

/**
 * @brief A thread-wrapping class that will forever attempt to receive
 *        new frames from a connected RTP Stream and feed them to a decoder
 */
class StreamSpinner {
 public:
  /**
   * @brief Construct a new Stream Spinner object
   *
   * @param decoder A decoder object
   * @param buffer A pointer to a circular array of decoded frame buffers
   * @param packet_getter A function to get new packets from teh stream
   */
  StreamSpinner(
      std::unique_ptr<decoder::FrameDecoder>&& decoder,
      const std::shared_ptr<decoder::CircularFrameBuffer> &buffer,
      std::function<std::unique_ptr<Packet>()> packet_getter);

  /**
   * @brief Destroy the StreamSpinner object
   */
  virtual ~StreamSpinner();

  /**
   * @brief Get the index of the most recently written buffer in the
   *        supplied CircularFrameBuffer. If a new frame has not been received
   *        since the last call, a negative index will be returned
   */
  int GetNextFrame();

 private:
  void Loop();

  std::unique_ptr<decoder::FrameDecoder> decoder_;
  std::function<std::unique_ptr<Packet>()> packet_getter_;
  std::shared_ptr<decoder::CircularFrameBuffer> frame_buffer_;

  std::atomic<bool> killed_;
  std::thread thread_;
};

}  // namespace codec
}  // namespace codec

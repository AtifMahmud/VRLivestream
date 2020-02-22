/**
 * @file packet.h
 * @brief Contains the Packet class declaration
 */

#pragma once

#include <cstdint>

extern "C" {
#include <libavcodec/avcodec.h>
}

namespace codec {

/**
 * @brief An RAII wrapper class around the AVPacket class
 */
class Packet {
 public:
  /**
   * @brief Construct an empty packet
   *
   * @throw std::runtime_error If the underlying ffmpeg object could not be allocated
   */
  Packet();

  /**
   * @brief Construct a new packet given a set of raw packet data
   *
   * @param raw_data The raw data
   * @param length The length of the raw data
   *
   * @throw std::runtime_error If the underlying ffmpeg object could not be allocated
   */
  Packet(uint8_t* raw_data, int length);

  // Delete the copy and assignment operators
  Packet(const Packet&) = delete;
  Packet operator=(const Packet&) = delete;

  /**
   * @brief Destroy the packet object
   */
  ~Packet();

  /**
   * @brief Get the underlying FFMpeg pointer
   */
  inline AVPacket * GetPacketPtr() { return packet_; }

  /**
   * @brief Get the size of the data
   */
  inline int Size() const { return packet_->size; }

  /**
   * @brief Get the underlying data of the packet
   */
  inline uint8_t* Data() { return packet_->data; }

 private:
  AVPacket* packet_;
};

}  // namespace codec

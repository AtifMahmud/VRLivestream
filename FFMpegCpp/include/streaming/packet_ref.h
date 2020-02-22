/**
 * @file packet_ref.h
 * @brief Holds declaration of the PacketRef class
 */

#pragma once

extern "C" {
#include "libavcodec/avcodec.h"
}

namespace codec {
namespace stream {

/**
 * @brief RAII Wrapper around creating a reference of an AVPacket
 */
class PacketRef {
 public:
  /**
   * @brief Construct a new PacketRef
   *
   * This will create a reference of the supplied packet that will be unreferenced
   * when this object is destructed
   *
   * @param src_packet The source packet to reference
   *
   * @throw CodecException If the packet could not be referenced
   */
  PacketRef(AVPacket* src_packet);
  virtual ~PacketRef();

  inline AVPacket* FFMpegPtr() { return &packet_; }

 private:
  AVPacket packet_;
};

}  // namespace stream
}  // namespace codec
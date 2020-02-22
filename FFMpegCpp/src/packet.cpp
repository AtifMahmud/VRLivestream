/**
 * @file packet.cpp
 * @brief Contains the implementation of the Pacet class
 */

#include "common/packet.h"

#include <stdexcept>

#include "common/codec_exception.h"

namespace codec {

Packet::Packet()
{
  packet_ = av_packet_alloc();
  if (!packet_)
    throw std::runtime_error("Packet: Cannot allocate memory for new packet");
}

Packet::Packet(uint8_t *const raw_data, const int length)
{
  packet_ = av_packet_alloc();
  if (!packet_)
    throw std::runtime_error("Packet: Cannot allocate memory for new packet");

  packet_->data = raw_data;
  packet_->size = length;
}

Packet::~Packet()
{
  av_packet_unref(packet_);
  av_packet_free(&packet_);
}

}  // namespace codec

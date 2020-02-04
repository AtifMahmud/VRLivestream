#pragma once

#include <cstdint>

extern "C" {
#include <libavcodec/avcodec.h>
}

namespace codec {

class Packet {
 public:
  Packet();
  Packet(uint8_t* raw_data, int length);
  Packet(const Packet& other) = delete;
  
  ~Packet();

  inline AVPacket * GetPacketPtr() { return packet_; }
  inline int Size() const { return packet_->size; }
  inline uint8_t* Data() { return packet_->data; }

 private:
  AVPacket* packet_;
};

}  // namespace codec

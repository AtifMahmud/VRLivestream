#include "common/packet.h"

#include "common/codec_exception.h"
#include "common/error_codes.h"

namespace codec {

Packet::Packet()
{
  packet_ = av_packet_alloc();
  if (!packet_)
    throw CodecException(
        ErrorCode::FAILED_ALLOC, 
        "Packet: Cannot allocate memory for new packet"); 
}

Packet::Packet(uint8_t *const raw_data, const int length)
{
  packet_ = av_packet_alloc();
  if (!packet_)
    throw CodecException(
        ErrorCode::FAILED_ALLOC, 
        "Packet: Cannot allocate memory for new packet");  
  
  packet_->data = raw_data;
  packet_->size = length;
}

Packet::~Packet()
{
  av_packet_unref(packet_);
  av_packet_free(&packet_);
}

}  // namespace codec

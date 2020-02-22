/**
 * @file packet_ref.cpp
 * @brief Implementation of the PacketRef class
 */

#include "streaming/packet_ref.h"

#include "common/codec_exception.h"

namespace codec {
namespace stream {

PacketRef::PacketRef(AVPacket *const src_packet)
{
  int err = av_packet_ref(&packet_, src_packet);
  if (err < 0)
    throw CodecException(err, "Could not reference packet");
}

PacketRef::~PacketRef()
{
  av_packet_unref(&packet_);
}

}  // namespace stream
}  // namespace codec

/**
 * @file rtp_server.cpp
 * @brief Implementatin of the RTPServer class
 */

#include <stdexcept>

#include "common/codec_exception.h"
#include "streaming/packet_ref.h"
#include "streaming/rtp_server.h"

namespace codec {
namespace stream {

RTPServer::RTPServer(const int fps, const int width, const int height)
{
  encoder_ = std::make_unique<encoder::FrameEncoder>(fps, width, height);
}

RTPServer::~RTPServer()
{}

void RTPServer::SendRawFrame(uint8_t *const raw_data, const int raw_length)
{
  std::lock_guard<std::mutex> lck(mutex_);

  std::vector<std::unique_ptr<Packet>> packets =
      encoder_->EncodeFrame(raw_data, raw_length);

  for (const auto &packet : packets) {
    if (packet == nullptr)  // should never happen
      throw std::runtime_error("Error: Unexpected null packet from encoder");

    for (const auto &stream : streams_) {
      PacketRef pck(packet->GetPacketPtr());
      stream->SendPacket(pck.FFMpegPtr());
    }
  }
}

std::string RTPServer::AddStream(const std::string &client_ip)
{
  std::lock_guard<std::mutex> lck(mutex_);

  auto stream = std::make_unique<RTPStream>(
      client_ip, CLIENT_PORT, encoder_->Codec(), encoder_->CodecContext());

  std::string sdp_content = stream->GetSdpText();
  streams_.emplace_back(std::move(stream));
  return sdp_content;
}

}  // namespace stream
}  // namespace codec

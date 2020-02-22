/**
 * @file rtp_client.cpp
 * @brief Implementation of the RTPClient class
 */

#include "streaming/rtp_client.h"

#include <functional>
#include <stdexcept>

#include "common/codec_exception.h"
#include "decoding/frame_decoder.h"

namespace codec {
namespace stream {

RTPClient::RTPClient(
    const std::string &sdp_file_content,
    const std::string &write_dir,
    const char dir_delim)
{
  int success = avformat_network_init();
  if (success != 0)
    throw CodecException(
        success, "Could not initialize network for receieving rtp stream");

  format_context_ = std::make_unique<ClientFormatContext>(
      sdp_file_content, write_dir, dir_delim);

  if (format_context_->GetNumStreams() <= 0)
    throw std::runtime_error("RTP connection has no valid streams");

  if (format_context_->GetNumStreams() > 1)
    throw std::runtime_error(
        "RTP connection has multiple streams. "
        "Only 1 video stream is currently suported.");

  circ_buff_ = std::make_shared<decoder::CircularFrameBuffer>();
}

RTPClient::~RTPClient()
{
  avformat_network_deinit();
}

void RTPClient::Start()
{
  auto frame_decoder =  std::make_unique<decoder::FrameDecoder>(
      OUT_PIX_FMT, format_context_->CodecID());

  spinner_ = std::make_unique<StreamSpinner>(
      std::move(frame_decoder),
      circ_buff_,
      std::bind(&RTPClient::ReceivePacket, this));
}

int RTPClient::GetCurrBuffIndex()
{
  if (!spinner_)
    throw std::runtime_error(
        "Cannot get the stream buffer before starting the client!");

  return spinner_->GetNextFrame();
}

std::unique_ptr<Packet> RTPClient::ReceivePacket()
{
  auto packet = std::make_unique<Packet>();

  int success = av_read_frame(
      format_context_->GetFFMpegPtr(), packet->GetPacketPtr());

  if (success != 0)
    return nullptr;

  return packet;
}

int RTPClient::GetBufferSize() const
{
  return decoder::DecodedFrameBuffer::GetRequiredBufferSize(
      format_context_->Width(),
      format_context_->Height(),
      OUT_PIX_FMT);
}

void RTPClient::AddBuffer(uint8_t *const buffer)
{
  circ_buff_->AddBuffer(
      buffer,
      format_context_->Width(),
      format_context_->Height(),
      OUT_PIX_FMT);
}

void RTPClient::GetFrameDimensions(int *const width, int *const height)
{
  if (!width || !height)
    throw std::invalid_argument(
        "Cannot set frame dimensions - supplied output pointers are null");

  *width = format_context_->Width();
  *height = format_context_->Height();
}

}  // namespace stream
}  // namespace codec

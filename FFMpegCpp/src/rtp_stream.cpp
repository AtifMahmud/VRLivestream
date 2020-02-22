/**
 * @file rtp_stream.cpp
 * @brief Implementation of the RTPStream class
 */

#include "streaming/rtp_stream.h"

extern "C" {
#include "libavformat/avformat.h"
}

#include <stdexcept>

#include "common/codec_exception.h"

namespace codec {
namespace stream {

RTPStream::RTPStream(
    const std::string &ip_address,
    const int port,
    AVCodec *const codec,
    AVCodecContext *const codec_context)
    : stream_(nullptr),
      codec_context_(codec_context)
{
  int success = avformat_network_init();
  if (success < 0)
    throw CodecException(success, "Error initializing network for rtp stream");

  format_context_ = std::make_unique<StreamFormatContext>(ip_address, port);

  stream_ = avformat_new_stream(format_context_->GetFFMpegPtr(), codec);
  if (!stream_)
    throw std::runtime_error("Could not open new stream object for rtp stream");

  success = avcodec_parameters_from_context(stream_->codecpar, codec_context_);
  if (success < 0)
    throw CodecException(
        success, "Could not read parameters from codec context for rtp stream");

  success = avformat_write_header(format_context_->GetFFMpegPtr(), NULL);
  if (success < 0)
    throw CodecException(success, "Cannot write header");

  sdp_file_ = std::make_unique<SDPFile>(format_context_.get());
}

RTPStream::~RTPStream()
{}

const char* RTPStream::GetSdpText()
{
  return sdp_file_->GetSdpText();
}

void RTPStream::SendPacket(AVPacket*const packet)
{
  av_packet_rescale_ts(
      packet,
      codec_context_->time_base,
      stream_->time_base);

  packet->stream_index = stream_->index;
  av_write_frame(format_context_->GetFFMpegPtr(), packet);
}

}  // namespace stream
}  // namespace codec

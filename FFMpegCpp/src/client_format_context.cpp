/**
 * @file client_format_context.cpp
 * @brief Implementation of the ClientFormatContext class
 */

#include "streaming/client_format_context.h"

#include "common/codec_exception.h"
#include "streaming/sdp_file.h"

namespace codec {
namespace stream {

ClientFormatContext::ClientFormatContext(
    const std::string &sdp_file_content,
    const std::string &write_dir,
    const char dir_delim)
    : format_context_(NULL)
{
  AVDictionary *d = NULL;
  int success = av_dict_set(&d, "protocol_whitelist", "file,udp,rtp", 0);
  if (success != 0)
    throw CodecException(success, "Could not set the input format whitelist");

  success = av_dict_set(&d, "buffer_size", "8000000", 0);
  if (success != 0)
    throw CodecException(success, "Could not set buffer size");

  SDPFile sdp_file(sdp_file_content, write_dir, dir_delim);

  success = avformat_open_input(
      &format_context_, sdp_file.Path().c_str(), NULL, &d);
  if (success != 0)
    throw CodecException(success, "Could not open input sdp");

  success = avformat_find_stream_info(format_context_, nullptr);
  if (success != 0)
    throw CodecException(
        success, "Could not find format stream info for rtp stream");

  AVCodecParameters* codec_params = format_context_->streams[0]->codecpar;
  if (codec_params == NULL)
    throw std::runtime_error("Could not open sdp file, codec is null");

  height_ = codec_params->height;
  width_ = codec_params->width;
  codec_id_ = codec_params->codec_id;

  if (height_ <= 0 || width_ <= 0)
    throw std::runtime_error(
        "Width or height of stream is invalid.\n"
        "Width: " + std::to_string(width_) + "\n"
        "Height: " + std::to_string(height_));
}

ClientFormatContext::~ClientFormatContext()
{
  avformat_close_input(&format_context_);
}

int ClientFormatContext::GetNumStreams() const
{
  return format_context_->nb_streams;
}

}  // namespace stream
}  // namespace codec

/**
 * @file stream_format_context.cpp
 * @brief Implementation of the StreamFormatContext class
 */

#include "streaming/stream_format_context.h"

#include <stdexcept>

#include "common/codec_exception.h"

namespace codec {
namespace stream {

StreamFormatContext::StreamFormatContext(
    const std::string &ip_address, const int port)
{
  AVOutputFormat* output_format = av_guess_format("rtp", nullptr, nullptr);
  if (!output_format)
    throw std::runtime_error("Could not allocate output format for rtp stream");

  const std::string url = "rtp://" + ip_address + ":" + std::to_string(port);
  int success = avformat_alloc_output_context2(
      &format_context_,
      output_format,
      output_format->name,
      url.c_str());

  if (success < 0)
    throw CodecException(
        success, "Could not allocate output context for rtp stream");

  success = avio_open(
      &(format_context_->pb), format_context_->url, AVIO_FLAG_WRITE);
  if (success < 0)
    throw CodecException(success, "Could not open io for rtp stream");
}

StreamFormatContext::~StreamFormatContext()
{
  avformat_free_context(format_context_);
}

}  // namespace stream
}  // namespace codec
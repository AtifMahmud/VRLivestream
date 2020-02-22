/**
 * @file rtp_stream.h
 * @brief Holds the declaration of the RTPStream class
 */

#pragma once

extern "C" {
#include "libavcodec/avcodec.h"
}

#include <memory>
#include <string>

#include "streaming/stream_format_context.h"
#include "streaming/sdp_file.h"

namespace codec {
namespace stream {

/**
 * @brief Represents an RTP stream for a single client
 */
class RTPStream {
 public:
  /**
   * @brief Construct a new RTPStream
   *
   * @param ip_address The ip address of the client
   * @param port The port to create the stream for
   * @param codec A pointer to the AVCodec object being used for encoding
   * @param codec_context A pointer to the AVCodecContext being used for encoding
   *
   * @throw std::runtime_error If the stream could not be created
   * @throw CodecException For any FFMpeg Library error
   */
  RTPStream(
      const std::string &ip_address,
      int port,
      AVCodec* codec,
      AVCodecContext* codec_context);

  // Delete the copy constructor and assignment operator
  RTPStream(const RTPStream&) = delete;
  RTPStream operator=(const RTPStream&) = delete;

  /**
   * @brief Destroy the RTPStream object
   */
  virtual ~RTPStream();

  /**
   * @brief Get the SdpFile content for the constructed stream
   */
  const char* GetSdpText();

  /**
   * @brief Send an encoded packet to the client
   */
  void SendPacket(AVPacket* packet);

 private:
  AVStream* stream_;
  AVCodecContext* codec_context_;
  std::unique_ptr<StreamFormatContext> format_context_;
  std::unique_ptr<SDPFile> sdp_file_;
};

}  // namesapce stream
}  // namespace codec

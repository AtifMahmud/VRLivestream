/**
 * @file stream_format_context.h
 * @brief Holds the declaration of the StreamFormatContext class
 */

#pragma once

extern "C" {
#include <libavformat/avformat.h>
}

#include <string>

namespace codec {
namespace stream {

/**
 * @brief An RAII wrapper class around opening a AVFormatContext class
 */
class StreamFormatContext {
 public:
  /**
   * @brief Construct a new StreamFormatContext object
   *
   * @throw CodecException On an FFMpeg library error
   * @throw std::runtime_error On any other error
   *
   * @param ip_address The ip address of the client
   * @param port The port to use for the stream
   */
  StreamFormatContext(const std::string &ip_address, int port);

  // Delete the copy constructor and the assignment operator
  StreamFormatContext(const StreamFormatContext&) = delete;
  StreamFormatContext operator=(const StreamFormatContext&) = delete;

  /**
   * @brief Destroy the StreamFormatContext object
   */
  virtual ~StreamFormatContext();

  /**
   * @brief Get the underlying AVFormatContext poiner
   */
  inline AVFormatContext* GetFFMpegPtr() { return format_context_; }

 private:
  AVFormatContext* format_context_;
};

}  // namespace stream
}  // namespace codec

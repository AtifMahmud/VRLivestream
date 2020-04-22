/**
 * @file client_format_context.h
 * @brief Holds the declaration of the ClientFormatContext class
 */

#pragma once

extern "C" {
#include <libavformat/avformat.h>
}

#include <string>

namespace codec {
namespace stream {

/**
 * @brief RAII wrapper around an FFMpeg AVFormatContext object
 *        Used to connect to the RTP stream of the server
 */
class ClientFormatContext {
 public:
  /**
   * @brief Construct a new Client Format Context object
   *
   * @param sdp_file_content The string content of the sdp file from the server
   *                         This will be written to a file so that it can be opened
   *                         by the FFMpeg library
   * @param write_dir A directory that the app has write permissions for
   * @param dir_delim The deliminator character for directories of the host OS
   *
   * @throw std::runtime_error If the sdp file could not be opened or the connected
   *        stream video dimensions are invalid
   * @throw CodecException On any FFMpeg library error
   */
  ClientFormatContext(
      const std::string &sdp_file_content,
      const std::string &write_dir,
      char dir_delim);

  // Delete the copy constructor and assignment operator
  ClientFormatContext(const ClientFormatContext&) = delete;
  ClientFormatContext operator=(const ClientFormatContext&) = delete;

  /**
   * @brief Destroy the ClientFormatContext object
   */
  virtual ~ClientFormatContext();

  /**
   * @brief Get they underlying FFMpeg pointer
   */
  inline AVFormatContext* GetFFMpegPtr() { return format_context_; }

  /**
   * @brief Get the number of streams in the connected rtp stream.
   *
   * A single connection can have multiple streams (eg. video, audio, etc)
   */
  int GetNumStreams() const;

  /**
   * @brief Get the width of the image frames for the connected stream
   */
  inline int Width() const { return width_; }

  /**
   * @brief Get the height of the image frames for the connected stream
   */
  inline int Height() const { return height_; }

  /**
   * @brief Get the id of the codec that was used to encode the video
   *        This will be needed in order to decode the video
   */
  inline AVCodecID CodecID() const { return codec_id_; }

 private:
  AVFormatContext* format_context_;
  int width_;
  int height_;
  AVCodecID codec_id_;
};

}  // namespace stream
}  // namespace codec

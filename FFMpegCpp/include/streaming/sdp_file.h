/**
 * @file sdp_file.h
 * @brief Holds the declaration of the SDPFile class
 */

#pragma once

#include <string>

#include "streaming/stream_format_context.h"

namespace codec {
namespace stream {

/**
 * @brief Wrapper around an SDPFile
 *
 * An sdp file is a streaming description protocol file that is given from the server
 * to the client. The client reads the sdp file in order to determine how to connect
 * to the stream.
 */
class SDPFile {
 public:
  /**
   * @brief Construct a new SDPFile using the server-side streaming format context
   *
   * This constructor will not phyiscall write a file to disc
   *
   * @param format_context The streaming format context
   *
   * @throw CodecException on an FFMpeg library exception
   */
  SDPFile(StreamFormatContext* format_context);

  /**
   * @brief Construct a new SDPFile object given the content of the sdp file
   *
   * This constructor will physically write an sdp file to disc
   *
   * @param content The content of the SDP file to write
   * @param write_dir A directory with app write permissions
   * @param dir_delim The directory seperator of the Host OS
   *
   * @throw std::runtime_error If the file could not be written to disc
   */
  SDPFile(
      const std::string &content,
      const std::string &write_dir,
      char dir_delim);

  /**
   * @brief Get the contents of the sdp file
   *
   * @return const char* The sdp file content
   */
  const char* GetSdpText();

  /**
   * @brief Destroy the SDPFile object
   *
   * In the case that the content was written into a physical file, this destructor
   * will delete it
   */
  virtual ~SDPFile();

  /**
   * @brief Get the path to the physical file. Will be an empty string if the
   *        file was not written to disc
   */
  inline std::string Path() const { return file_name_; }

 private:
  std::string file_name_;
  std::string sdp_text_;
  bool file_created_;
};

}  // namespace stream
}  // namespace codec

/**
 * @file sdp_file.cpp
 * @brief Implementation of the SDPFile class
 */

#include "streaming/sdp_file.h"

extern "C" {
#include <libavformat/avformat.h>
}

#include <fstream>

#include "common/codec_exception.h"

constexpr int BUFFER_SIZE = 200000;

namespace codec {
namespace stream {

SDPFile::SDPFile(StreamFormatContext *const format_context)
    : file_created_(false)
{
  file_name_ = "";

  char buffer[BUFFER_SIZE];
  AVFormatContext* context = format_context->GetFFMpegPtr();
  int success = av_sdp_create(&context, 1, buffer, BUFFER_SIZE);
  if (success != 0)
    throw CodecException(
        success, "Could create sdp file for rtp stream");

  sdp_text_.assign(buffer);
}

SDPFile::SDPFile(
    const std::string &content,
    const std::string &write_dir,
    const char dir_delim)
    : file_created_(true)
{
  file_name_ = write_dir + dir_delim + dir_delim + "vr_livestream.sdp";

  std::ofstream sdp_file(file_name_);
  if (sdp_file.fail())
    throw std::runtime_error("Could not write sdp data to file for rtp stream");

  sdp_file << content;
}

SDPFile::~SDPFile()
{
  if (file_created_)
    std::remove(file_name_.c_str());
}

const char* SDPFile::GetSdpText()
{
  return sdp_text_.c_str();
}

}  // namespace stream
}  // namespace codec

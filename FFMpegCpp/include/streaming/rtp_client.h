/**
 * @file rtp_client.h
 * @brief Holds the declaration of the RTPClient class
 */

#pragma once

extern "C"
{
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
}

#include <memory>
#include <string>

#include "common/packet.h"
#include "decoding/circular_frame_buffer.h"
#include "streaming/client_format_context.h"
#include "streaming/stream_spinner.h"

namespace codec {
namespace stream {

/**
 * @brief Main class for the RTP Client connection
 *
 * The C# code should add buffers to class to hold decoded frames data and then
 * start the stream.
 *
 * The index of the current frame buffer can then be retrieved by calling
 * GetCurrBuffIndex. The C# code must keep track of the order in which they
 * added the buffers in order to map the index to the C# buffer
 *
 * This class will spin a separate thread and continuously attempt to retreive
 * new frames from the stream and decode them. This helps ensure that the decoder
 * receives as many frames as possible.
 */
class RTPClient {
 public:
  /**
   * @brief Construct a new RTPClient object
   *
   * @param sdp_file_content The sdp file that describes how to connect to the server's stream
   * @param write_dir A directory that the app has write permissions for
   *                  in orde to write the sdp file content to a file on the disc
   * @param dir_delim The deliminator for separating directories for the host OS
   *
   * @throw std::runtime_error If the stream could not be connected to or is invalid
   * @throw CodecException On any FFMpeg library error
   */
  RTPClient(
      const std::string &sdp_file_content,
      const std::string &write_dir,
      char dir_delim);

  /**
   * @brief Destroy the RTPClient object
   */
  virtual ~RTPClient();

  /**
   * @brief Start reading frames from the stream
   */
  void Start();

  // Delete the copy constructor and the assignment operator
  RTPClient(const RTPClient&) = delete;
  RTPClient operator=(const RTPClient&) = delete;

  /**
   * @brief Get the index of the buffer that contains the most recently captured
   *        and decoded image frames
   *
   * @throw std::runtime_error If the client has not been started
   *
   * @return int The index of the buffer, or a negative number if no new frame has
   *             been received since the last call to this function
   */
  int GetCurrBuffIndex();

  /**
   * @brief Get the required buffer size needed to hold a single decoded image frame
   */
  int GetBufferSize() const;

  /**
   * @brief Add a buffer to hold decoded image data. The buffer must be of the size
   *        returned by the call to GetBufferSize
   */
  void AddBuffer(uint8_t* buffer);

  /**
   * @brief Get the dimensions of the connected video stream, in pixels
   *
   * @throw std::invalid_argument If the supplied output pointers are null
   *
   * @param width Output ptr that will be updated with the height of the stream
   * @param height Output ptr that will be updated with the width of the stream
   */
  void GetFrameDimensions(int *width, int *height);

 private:
  std::unique_ptr<Packet> ReceivePacket();

  std::unique_ptr<ClientFormatContext> format_context_;
  std::unique_ptr<StreamSpinner> spinner_;
  std::shared_ptr<decoder::CircularFrameBuffer> circ_buff_;

  // The output pixel format for Unity.
  // This must match exactly the pixel format that is expected by Unity
  const AVPixelFormat OUT_PIX_FMT = AVPixelFormat::AV_PIX_FMT_RGBA;
};

}  // namespace stream
}  // namespace codec

/**
 * @file rtp_server.h
 * @brief Holds the RTPServer class
 */

#pragma once

#include <memory>
#include <mutex>
#include <vector>

#include "encoding/frame_encoder.h"
#include "streaming/rtp_stream.h"

namespace codec {
namespace stream {

/**
 * @brief Main class of the RTP server
 *
 * This class will internally hold an encoder to encode raw image frames
 * The C# code should add any clients by calling AddStream. On this call,
 * a new RTP stream will be created. On a call to SendRawFrame, the server
 * will encode the frame into a packet, and send one copy of the packet to every
 * client.
 */
class RTPServer {
 public:
  /**
   * @brief Construct a new RTPServer object
   *
   * @param fps The desired fps of the stream
   * @param width The width of raw image frames
   * @param height The height of raw image frames
   */
  RTPServer(int fps, int width, int height);

  /**
   * @brief Destroy the RTPServer object
   */
  virtual ~RTPServer();

  /**
   * @brief Add a new client to stream video to
   *
   * @param client_ip The ip address of the client
   *
   * @return std::string A string holding the contents of an sdp file.
   *                     The C# code should send this sdp file to the client
   *                     in order for the client to connect to the stream
   */
  std::string AddStream(const std::string &client_ip);

  /**
   * @brief Send a frame to every connected client
   *
   * This function will encode the frame and then send the resulting encoded
   * packets to every connected client
   *
   * @throw CodecException On an FFMpeg library error
   * @throw std::runtime_error On any other error
   *
   * @param raw_data The raw frame data
   * @param raw_length The length of the raw frame data
   */
  void SendRawFrame(uint8_t* raw_data, int raw_length);

 private:
  std::unique_ptr<encoder::FrameEncoder> encoder_;
  std::vector<std::unique_ptr<RTPStream>> streams_;
  std::mutex mutex_;

  // The port number of the client.
  // The client will read this from an SDP file, so it simply needs
  // to be an arbitrary port number that isn't "well-known"
  const int CLIENT_PORT = 40000;
};

}  // namespace stream
}  // namespace codec

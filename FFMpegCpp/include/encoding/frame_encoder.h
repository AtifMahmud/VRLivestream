/**
 * @file frame_encoder.h
 * @brief Holds the declaration of the FrameEncoder class
 */

#pragma once

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
}

#include <memory>
#include <vector>

#include "common/packet.h"
#include "common/pixel_converter.h"
#include "encoding/encoder_context.h"
#include "encoding/encoder_frame.h"

namespace codec {
namespace encoder {

/**
 * @brief The main class for encoding raw image frames into encoded packets
 */
class FrameEncoder {
 public:
  /**
   * @brief Construct a new FrameEncoder
   *
   * @param fps The desired fps
   * @param width The width of each image frame
   * @param height The height of each image frame
   * @param codec_speed for example, "ultrafast", "medium". (https://trac.ffmpeg.org/wiki/Encode/H.264)
   * @param codec_type select between h264 and h265. values can be found in frame_encoder.h
   */
  FrameEncoder(int fps, int width, int height, char* codec_speed, int codec_type);

  /**
   * @brief Destroy the FrameEncoder
   */
  virtual ~FrameEncoder() {}

  /**
   * @brief Encode a single frame of data and receive the encoded packets.
   *
   * A single packet may be returned for one image frame, multiple packets, or no packets
   *
   * @param raw_data The raw data to encode
   * @param raw_length The raw length of the data
   *
   * @throw std::runtime_error If the supplied data is null,
   *        or the length of the data is invalid
   * @throw CodecException On an FFMpeg library error
   *
   * @return std::vector<std::unique_ptr<Packet>> A vector of encoded packets
   */
  std::vector<std::unique_ptr<Packet>> EncodeFrame(
      uint8_t* raw_data, int raw_length);

  /**
   * @brief Get the underlying FFMpeg AVCodec pointer
   */
  inline AVCodec* Codec() { return encoder_context_->GetCodecPtr(); }

  /**
   * @brief Get the underlying FFMpeg AVCodecContext pointer
   */
  inline AVCodecContext* CodecContext()
      { return encoder_context_->GetContextPtr(); }

 private:
  std::unique_ptr<EncoderContext> encoder_context_;
  std::unique_ptr<EncoderFrame> encoder_frame_;
  std::unique_ptr<PixelConverter> pixel_converter_;

  int height_;

  // The pixel format expected from Unity. This must match exactly with what
  // is being extracted in the Unity C# code
  const AVPixelFormat SRC_PIXEL_FORMAT = AVPixelFormat::AV_PIX_FMT_RGBA;
};

namespace detail {

AVFrame FrameFromRawData(
    uint8_t* raw_data,
    int raw_length,
    AVPixelFormat format,
    int height);

}  // namespace detail

}  // encoder
}  // namespace codec

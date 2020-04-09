/**
 * @file encoder_context.h
 * @brief Holds the EncoderContext class
 */

#pragma once

// different codec ids
#define H264 0
#define H265 1

extern "C" {
#include <libavcodec/avcodec.h>
}

namespace codec {
namespace encoder {

/**
 * @brief An RAII wrapper around an FFMpeg AVCodec and AVCodecContext for encoding
 */
class EncoderContext {
 public:
  /**
   * @brief Construct a new EncoderContext
   *
   * @param fps The desired frames per second
   * @param width The width of each image frame
   * @param height The height of each image frame
   * @param codec_speed for example, "ultrafast", "medium". (https://trac.ffmpeg.org/wiki/Encode/H.264)
   * @param codec_type select between h264 and h265. values can be found in frame_encoder.h
   *
   * @throw std::runtime_error If the encoder codec was not found or if the
   *        underlying FFMpeg object could not be allocated
   * @throw CodecException If the codec could not be opened
   */
  EncoderContext(int fps, int width, int height, char* codec_speed, int codec_type);

  // Delete the copy constructor and assignment operator
  EncoderContext(const EncoderContext&) = delete;
  EncoderContext operator=(const EncoderContext&) = delete;

  /**
   * @brief Destroy the EncoderContext object
   */
  ~EncoderContext();

  // Getters for underling FFMpeg objects
  inline AVPixelFormat GetPixelFormat() const {
    return codec_context_ptr_->pix_fmt;
  }
  inline AVCodecContext* GetContextPtr() { return codec_context_ptr_; }
  inline AVCodec* GetCodecPtr() { return codec_ptr_; }

 private:
  AVCodec*        codec_ptr_;
  AVCodecContext* codec_context_ptr_;

  // The pixel format to encode image frames in
  // The H264 encoder does not directly support RGB pixel formats
  // So use a YUV pixel format
  const AVPixelFormat PIX_FMT = AVPixelFormat::AV_PIX_FMT_YUV420P;
};

}  // encoder
}  // namespace codec

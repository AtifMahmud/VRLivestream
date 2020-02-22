/**
 * @file decoder_context.h
 * @brief Holds the declaration of the DecoderContext class
 */

#pragma once

extern "C" {
#include <libavcodec/avcodec.h>
}

namespace codec {
namespace decoder {

/**
 * @brief An RAII wrapper around an FFMpeg AVCodec and AVCodecContext object
 */
class DecoderContext {
 public:
  /**
   * @brief Construct a new DecoderContext object.
   *
   * @param codec_id The id of the Codec to use for decoding
   *
   * @throw std::runtime_error If the codec or context could not be allocated
   * @throw CodecException If the codec could not be opened
   */
  DecoderContext(AVCodecID codec_id);

  // Delete the copy and assignment operators
  DecoderContext(const DecoderContext&) = delete;
  DecoderContext operator=(const DecoderContext&) = delete;

  /**
   * @brief Destroy the DecoderContext object
   */
  ~DecoderContext();

  /**
   * @brief Get the underlying FFMpeg pointer
   */
  inline AVCodecContext* GetContext() { return codec_context_ptr_; }

 private:
  AVCodec*        codec_ptr_;
  AVCodecContext* codec_context_ptr_;
};

}  // namespace decoder;
}  // namespace codec

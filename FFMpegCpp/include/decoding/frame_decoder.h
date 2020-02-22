/**
 * @file frame_decoder.h
 * @brief Holds the declaration of the FrameDecoder class
 */

#pragma once

extern "C" {
#include <libavcodec/avcodec.h>
}

#include <memory>
#include <vector>

#include "decoding/decoded_frame_buffer.h"
#include "decoding/decoder_context.h"
#include "decoding/decoder_frame.h"
#include "common/packet.h"
#include "common/pixel_converter.h"

namespace codec {
namespace decoder {

/**
 * @brief Main class used for decoding. Can be passed encoded image packets
 *        and will return decoced image frames
 */
class FrameDecoder {
 public:
  /**
   * @brief Construct a new Frame Decoder object
   *
   * @param out_pxl_fmt The pixel format that the decoded images should have
   * @param codec_id The id of the codec to use for decoding.
   *                 It must match the codec used for encoding
   */
  FrameDecoder(
      AVPixelFormat out_pxl_fmt,
      AVCodecID codec_id);


  // Delete the copy constructor and assignment operator
  FrameDecoder(const FrameDecoder&) = delete;
  FrameDecoder operator=(const FrameDecoder&) = delete;

  /**
   * @brief Decode an encoded packet
   *
   * Not every packet will necessarily result in a decoded image frame,
   * and some may result in multiple frames
   *
   * ReceiveFrames should be called after this function until no more frmeas
   * can be receieved
   *
   * @param packet The packet to decode
   *
   * @throw CodecException If the packet could not be passed to the FFMpeg decoder
   *        or if ther was an error retreiving the frame from FFMpeg
   */
  void DecodeFrame(Packet* packet);

  /**
   * @brief Receive decoded image frames
   *
   * This function should be called after DecodeFrame and should be called
   * repeatedly until no more frames are returned
   *
   * @param buffer The buffer to store the decoded frame in
   * @return true If decoded frame was received, false otherwise
   */
  bool ReceiveFrames(DecodedFrameBuffer* buffer);

 private:
  std::unique_ptr<DecoderContext> decoder_context_;
  DecoderFrame decoder_frame_;
  std::vector<std::unique_ptr<DecodedFrameBuffer>> buffers_;
  std::unique_ptr<PixelConverter> pixel_converter_;
  AVPixelFormat out_pxl_fmt_;
};

}  // namespace decoder
}  // namespace codec

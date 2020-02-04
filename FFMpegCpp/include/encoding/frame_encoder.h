#pragma once

extern "C" {
#include <libavcodec/avcodec.h>
}

#include <memory>
#include <vector>

#include "common/packet.h"
#include "common/pixel_converter.h"
#include "encoding/encoder_context.h"
#include "encoding/encoder_frame.h"

namespace codec {
namespace encoder {

class FrameEncoder {
 public:
  FrameEncoder(int fps, int width, int height, int max_packets);
  virtual ~FrameEncoder() {}

  int EncodeFrame(
      uint8_t* raw_data, 
      int raw_length,
      uint8_t** encoded_data,
      int* encoded_length);

 private:
  std::unique_ptr<EncoderContext> encoder_context_;
  std::unique_ptr<EncoderFrame> encoder_frame_;
  std::unique_ptr<PixelConverter> pixel_converter_;
  std::vector<Packet> packets_;

  int width_;
  int height_;
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

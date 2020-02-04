#pragma once

extern "C" {
#include <libavcodec/avcodec.h>
}

#include <vector>

#include "decoding/decoded_frame_buffer.h"
#include "decoding/decoder_context.h"
#include "decoding/decoder_frame.h"
#include "common/packet.h"
#include "common/pixel_converter.h"

#include <memory>
#include <vector>

namespace codec {
namespace decoder {

class FrameDecoder {
 public:
  int DecodeFrame(
      uint8_t* raw_data, 
      int raw_length);
  
  int GetBufferSize(int image_width, int image_height) const;

  int AddBuffer(
      uint8_t* buffers, 
      const int width, 
      const int height);

 private:
  DecoderContext decoder_context_;
  DecoderFrame   decoder_frame_;
  std::vector<std::unique_ptr<DecodedFrameBuffer>> buffers_;
  std::unique_ptr<PixelConverter> pixel_converter_;
  const AVPixelFormat OUT_PIX_FMT = AVPixelFormat::AV_PIX_FMT_RGBA;
};

}  // namespace decoder
}  // namespace codec


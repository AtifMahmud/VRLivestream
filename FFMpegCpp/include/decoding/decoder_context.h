#pragma once

extern "C" {
#include <libavcodec/avcodec.h>
}

namespace codec {
namespace decoder {

class DecoderContext {
 public:
  DecoderContext();
  DecoderContext(const DecoderContext &other) = delete;

  ~DecoderContext();

  inline AVCodecContext* GetContext() { return codec_context_ptr_; }

 private:
  AVCodec*        codec_ptr_;
  AVCodecContext* codec_context_ptr_;
};

}  // namespace decoder;
}  // namespace codec

#pragma once

extern "C" {
#include <libavcodec/avcodec.h>
}

namespace codec {
namespace encoder {

class EncoderContext {
 public:
  EncoderContext(int fps, int width, int height);
  EncoderContext(const EncoderContext &other) = delete;

  ~EncoderContext();

  inline AVPixelFormat GetPixelFormat() const { 
    return codec_context_ptr_->pix_fmt; 
  }

  inline AVCodecContext* GetContextPtr() { return codec_context_ptr_; }

 private:
  AVCodec*        codec_ptr_;
  AVCodecContext* codec_context_ptr_;
  const AVPixelFormat PIX_FMT = AVPixelFormat::AV_PIX_FMT_YUV420P;   
};

}  // encoder
}  // namespace codec

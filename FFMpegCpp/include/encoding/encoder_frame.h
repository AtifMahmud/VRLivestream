#pragma once

extern "C" {
#include <libavcodec/avcodec.h>
}

#include "common/pixel_converter.h"

namespace codec {
namespace encoder {

class EncoderFrame {
 public:
  EncoderFrame(AVPixelFormat pixel_format, int width, int height);
  EncoderFrame(const EncoderFrame &other) = delete;
  
  ~EncoderFrame();  

  int EnsureWritable();
  void IncrementFrameCount();
  void SetData(AVFrame* src_frame, const PixelConverter &converter);
  inline AVFrame* GetFramePtr() { return frame_; }

 private:
  AVFrame* frame_;
  AVPixelFormat pixel_format_;
  int frame_count_;
};

}  // encoder
}  // namespace codec

#pragma once

extern "C" {
#include <libavcodec/avcodec.h>
}

namespace codec {
namespace decoder {

class DecodedFrameBuffer {
 public:
  DecodedFrameBuffer(int width, int height, AVPixelFormat pxl_fmt);
  DecodedFrameBuffer(const DecodedFrameBuffer &other) = delete;
  
  ~DecodedFrameBuffer();
  
  int SetBuffer(uint8_t* buffer);
  
  inline uint8_t** GetData() { return frame_->data; }
  inline int* GetLineSize() { return frame_->linesize; }

  static int GetRequiredBufferSize(
      int width, int height, AVPixelFormat pxl_fmt);

 private:
  int width_;
  int height_;
  AVPixelFormat pxl_fmt_;
  AVFrame* frame_;
};

}  // namespace decoder
}  // namespace codec

#pragma once

extern "C" {
#include <libavcodec/avcodec.h>
}

namespace codec {
namespace decoder {

class DecoderFrame {
 public:
  DecoderFrame();
  DecoderFrame(const DecoderFrame &other) = delete;

  ~DecoderFrame();

  inline AVFrame* GetFramePtr() { return frame_; }
  inline uint8_t ** Data()      { return frame_->data; }  
  inline int * LineSize()       { return frame_->linesize; }  
  inline int Width() const      { return frame_->width; }
  inline int Height() const     { return frame_->height; }
  inline int DataSize() const   { return frame_->height * frame_->linesize[0];}
  
  inline AVPixelFormat PixelFormat() const { 
    return static_cast<AVPixelFormat>(frame_->format); 
  }

 private:
  AVFrame* frame_;
};

}  // namespace decoder
}  // namespace codec

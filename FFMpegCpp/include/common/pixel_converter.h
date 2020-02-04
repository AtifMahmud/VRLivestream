#pragma once

extern "C" {
#include <libavcodec/avcodec.h>
#include <libswscale/swscale.h>
}

namespace codec {

class PixelConverter {
 public:
  PixelConverter(
      AVPixelFormat src_format, 
      AVPixelFormat dest_format,
      int width, 
      int height);

  PixelConverter(const PixelConverter &other) = delete;

  ~PixelConverter();

  void ConvertPixels(
      const AVFrame* src_frame, 
      uint8_t ** dest_data, 
      int* dest_line_size) const;

  inline AVPixelFormat SrcFormat() const  { return src_format_; }
  inline AVPixelFormat DestFormat() const { return dest_format_; }

 private:
  AVPixelFormat src_format_;
  AVPixelFormat dest_format_;
  int           height_; 
  SwsContext*   sws_context_;
};

}  // namespace codec

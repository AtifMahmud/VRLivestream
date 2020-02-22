/**
 * @file pixel_converter.h
 * @brief Contains the PixelConverter class used by the encoder and decoder
 */

#pragma once

extern "C" {
#include <libavcodec/avcodec.h>
#include <libswscale/swscale.h>
}

namespace codec {

/**
 * @brief Class used to convert between pixel types
 *
 * This class is necessary in order to convert between Unity's pixel format
 * and the pixel format needed by the Encoding/Decoding library
 */
class PixelConverter {
 public:

  /**
   * @brief Construct a new Pixel Converter object
   *
   * @param src_format The source pixel format for conversions
   * @param dest_format  The destination pixel format for the conversions
   * @param width The width of image frames being converted
   * @param height The height of image frames being converted
   *
   * @throw std::runtime_error If the underlying FFMpeg object could not be allocated
   */
  PixelConverter(
      AVPixelFormat src_format,
      AVPixelFormat dest_format,
      int width,
      int height);

  // Delete the copy and assignment constructors - class contains raw pointers
  PixelConverter(const PixelConverter&) = delete;
  PixelConverter operator=(const PixelConverter&) = delete;

  /**
   * @brief Destroy the Pixel Converter object
   */
  ~PixelConverter();

  /**
   * @brief Converts pixels
   *
   * @param src_frame The source frame
   * @param dest_data Pointer to store the converted output
   * @param dest_line_size Pointer to store the converted line size
   */
  void ConvertPixels(
      const AVFrame* src_frame,
      uint8_t ** dest_data,
      int* dest_line_size) const;

  /**
   * @brief Read the source format for the pixel converter
   */
  inline AVPixelFormat SrcFormat() const  { return src_format_; }

  /**
   * @brief Read the destination format for the pixel converter
   *
   * @return AVPixelFormat
   */
  inline AVPixelFormat DestFormat() const { return dest_format_; }

 private:
  AVPixelFormat src_format_;
  AVPixelFormat dest_format_;
  int           height_;
  SwsContext*   sws_context_;
};

}  // namespace codec

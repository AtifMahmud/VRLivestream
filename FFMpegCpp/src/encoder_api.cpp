#include "common/codec_exception.h"
#include "common/error_codes.h"
#include "encoding/encoder_api.h"

// Unity Interface
extern "C" {

codec::encoder::FrameEncoder* CreateEncoder(
    int fps, int width, int height, int max_packets)
{
  try {
    auto encoder = new codec::encoder::FrameEncoder(fps, width, height, max_packets);
    return encoder;
  } catch (...) {
    return nullptr;
  }
}

void DestroyEncoder(codec::encoder::FrameEncoder* encoder)
{
  // TODO Fix this - it causes a crash
  // if (encoder) delete encoder;
}

int EncodeFrame(
    codec::encoder::FrameEncoder* encoder,
    uint8_t* raw_data, 
    int raw_length,
    uint8_t** decoded_data,
    int* decoded_length)
{
  try {
    if (!encoder) 
      return codec::ErrorCode::NULL_CODEC;
    return encoder->EncodeFrame(
        raw_data, raw_length, decoded_data, decoded_length);
  } catch (const codec::CodecException &e) {
    return e.ErrorCode();
  } catch (...) {
    return codec::ErrorCode::UNEXPECTED_EXCEPTION;
  }
}

}  // extern
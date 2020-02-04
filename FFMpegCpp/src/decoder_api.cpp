#include "decoding/decoder_api.h"

#include "common/codec_exception.h"
#include "common/error_codes.h"

// Unity Interface
extern "C" {

codec::decoder::FrameDecoder* CreateDecoder()
{
  try {
    auto decoder = new codec::decoder::FrameDecoder();
    return decoder;
  } catch (...) {
    return nullptr;
  }
}

void DestroyDecoder(codec::decoder::FrameDecoder* decoder)
{
  // TODO Fix this - It causes a crash
  // if (decoder) delete decoder;
}

int DecodeFrame(
    codec::decoder::FrameDecoder* decoder,
    uint8_t* raw_data, 
    int raw_length)
{
  try {
    if (decoder)
      return decoder->DecodeFrame(raw_data, raw_length);
    else
      return codec::ErrorCode::NULL_CODEC;
  } catch (const codec::CodecException &e) {
    return e.ErrorCode();
  } catch (...) {
    return codec::ErrorCode::UNEXPECTED_EXCEPTION;
  }
}

int GetBufferSize(
    codec::decoder::FrameDecoder* decoder, 
    int image_width, 
    int image_height)
{
  try {
    if (decoder == nullptr) 
      return codec::ErrorCode::NULL_CODEC;
    else
      return decoder->GetBufferSize(image_width, image_height);
  } catch (const codec::CodecException &e) {
    return e.ErrorCode();
  } catch (...) {
    return codec::ErrorCode::UNEXPECTED_EXCEPTION;
  }
}

int AddBuffer(
    codec::decoder::FrameDecoder *decoder, 
    uint8_t* buffer,
    const int width, 
    const int height)
{
  try {
    if (decoder == nullptr) 
      return codec::ErrorCode::UNEXPECTED_EXCEPTION;
    else
      return decoder->AddBuffer(buffer, width, height);
  } catch (const codec::CodecException &e) {
    return e.ErrorCode();
  } catch (...) {
    return codec::ErrorCode::UNEXPECTED_EXCEPTION;
  }
}

}  // extern

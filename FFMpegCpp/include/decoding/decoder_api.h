#pragma once

#include "decoding/frame_decoder.h"

#define EXPORT __attribute__((visibility("default")))
// use __declspec(dllexport) for windows

extern "C" {

EXPORT codec::decoder::FrameDecoder* CreateDecoder();

EXPORT void DestroyDecoder(codec::decoder::FrameDecoder* decoder);

EXPORT int DecodeFrame(
    codec::decoder::FrameDecoder* decoder,
    uint8_t* raw_data, 
    int raw_length);
    
EXPORT int GetBufferSize(
    codec::decoder::FrameDecoder* decoder, 
    int image_width_, 
    int image_height_);

EXPORT int AddBuffer(
    codec::decoder::FrameDecoder *decoder, 
    uint8_t* buffers,
    int width, 
    int height);

}  // extern

#pragma once

#include "encoding/frame_encoder.h"

#ifdef _WIN32
#define EXPORT __declspec(dllexport)
#else
#define EXPORT __attribute__((visibility("default")))
#endif

extern "C" {

EXPORT codec::encoder::FrameEncoder* CreateEncoder(
    int fps, int width, int height, int max_packets);
EXPORT void DestroyEncoder(codec::encoder::FrameEncoder* encoder);
EXPORT int EncodeFrame(
    codec::encoder::FrameEncoder* encoder,
    uint8_t* raw_data, 
    int raw_length,
    uint8_t** decoded_data,
    int* decoded_length);

}  // extern

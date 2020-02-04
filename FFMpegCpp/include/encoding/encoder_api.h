#pragma once

#include "encoding/frame_encoder.h"

#define EXPORT __attribute__((visibility("default")))
// use __declspec(dllexport) for windows

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
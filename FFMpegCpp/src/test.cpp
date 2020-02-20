#include <iostream>
#include "encoding/encoder_api.h"
#include "decoding/decoder_api.h"

int main() {
    codec::encoder::FrameEncoder* encoder = CreateEncoder(30, 100, 100, 5);
    codec::decoder::FrameDecoder* decoder = CreateDecoder();
}

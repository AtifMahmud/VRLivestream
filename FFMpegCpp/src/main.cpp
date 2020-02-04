#include "encoding/encoder_api.h"
#include "decoding/decoder_api.h"

#include <iostream>
int main()
{
  auto encoder = CreateEncoder(10, 1024, 1024, 5);
  auto decoder = CreateDecoder();

  uint8_t frame[1048576]; 
  int encoded_length;
  uint8_t* encoded_data;
  int success = encoder->EncodeFrame(frame, 1024 * 1024, &encoded_data, &encoded_length);
  if (success > 0) {
    std::cout << "Success encoding packet, now trying decode" << std::endl;
    int buffersize = decoder->GetBufferSize(1024, 1024);
    uint8_t* buffer = new uint8_t[buffersize];
    decoder->AddBuffer(buffer, 1024, 1024);
    success = decoder->DecodeFrame(encoded_data, encoded_length);
    if (success) std::cout << "Decode Success" << std::endl;
    else std::cout << "Decode fail" << std::endl;
  }
  else std::cout << "Encode fail" << std::endl;
}


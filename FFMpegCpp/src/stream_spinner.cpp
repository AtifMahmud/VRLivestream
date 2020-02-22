/**
 * @file stream_spinner.cpp
 * @brief Implementation of the StreamSpinner class
 */

#include "streaming/stream_spinner.h"
#include "decoding/decoded_frame_buffer.h"

namespace codec {
namespace stream {

StreamSpinner::StreamSpinner(
      std::unique_ptr<decoder::FrameDecoder>&& decoder,
      const std::shared_ptr<decoder::CircularFrameBuffer> &buffer,
      std::function<std::unique_ptr<Packet>()> packet_getter)
      : decoder_(std::move(decoder)),
        frame_buffer_(buffer),
        packet_getter_(packet_getter),
        killed_{false}
{
  thread_ = std::thread { std::bind(&StreamSpinner::Loop, this) };
}

StreamSpinner::~StreamSpinner()
{
  killed_.store(true);

  if (thread_.joinable())
    thread_.join();
}

void StreamSpinner::Loop()
{
  while (!killed_.load()) {
    try {
      std::unique_ptr<Packet> pck = packet_getter_();
      if (pck != nullptr) {
        decoder_->DecodeFrame(pck.get());
        while (true) {
          decoder::DecodedFrameBuffer* buf = frame_buffer_->StartWrite();
          if (decoder_->ReceiveFrames(buf))
            frame_buffer_->WriteSuccess();
          else
            break;
        }
      }
    } catch (...)
    {}  // This is in seperate thread - can't let exception propogate
  }
}

int StreamSpinner::GetNextFrame()
{
  return frame_buffer_->SetReadBuffer();
}

}  // namespace stream
}  // namespace codec

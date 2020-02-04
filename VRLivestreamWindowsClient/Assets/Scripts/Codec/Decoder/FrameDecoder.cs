using System;
using System.Collections.Generic;
using FFmpeg;
using FFmpeg.AutoGen;

namespace Codec {
namespace Decoder {

/// <summary>
/// Class to handle decoding of encoded video frames to raw data convertable to a Unity texture
/// </summary>
/// <remarks>
/// The output frames will be in RGBA pixel format to match the unity texture type
/// </remarks>
public class FrameDecoder {
    private readonly DecoderContext decoder_context_;
    private          DecoderFrame   yuv_frame_;
    private          RawFrame       rgb_frame_;
    private          PixelConverter pixel_conveter_;
    private const    AVPixelFormat  OUT_PIX_FMT = AVPixelFormat.AV_PIX_FMT_RGBA;

    /// <summary>
    /// Create the frame decoder
    /// </summary>
    public FrameDecoder()
    {
        // Set the directory where the DLLs exist
        ffmpeg.RootPath = Environment.CurrentDirectory + "\\Assets\\FFmpeg\\bin\\x86\\";

        decoder_context_ = new DecoderContext();
        yuv_frame_      =  new DecoderFrame();
    }

     /// <summary>
    /// Decode a video frame.
    /// </summary>
    /// <param name="encoded_packet">The encoded packet</param>
    /// <returns>A byte array representing the raw unencoded frame data</returns>
    /// <exception cref="Codec.FFMpegException">
    /// Thrown if the packet could not be decoded
    /// </exception>
    public byte[] DecodeFrame(Packet encoded_packet) {  // todo only take in a single packet
        int success = 0;
        CodecUtility.SendPacket(decoder_context_, encoded_packet);

        byte[] frame = new byte[0];
        while (success == 0) {
            success = CodecUtility.ReceiveFrame(decoder_context_, yuv_frame_);
            if (success == 0) {
                // Can only make the pixel converter and rgb frame once we know the dimensions of the frame
                if (pixel_conveter_ == null)
                    pixel_conveter_ =  new PixelConverter(
                            (AVPixelFormat) yuv_frame_.PixelFormat(),
                            OUT_PIX_FMT,
                            yuv_frame_.Width(),
                            yuv_frame_.Height());

                if (rgb_frame_ == null)
                    rgb_frame_ = new RawFrame(
                            OUT_PIX_FMT,
                            yuv_frame_.Width(),
                            yuv_frame_.Height());

                rgb_frame_.SetData(yuv_frame_, pixel_conveter_);
                frame = rgb_frame_.CopyToManagedMem();
                // Unity needs a bottom-up texture instead of a top-up texture
                frame = Codec.CodecUtility.GetFlippedArray(frame, yuv_frame_.Height(), frame.Length / yuv_frame_.Height());
            }
            else if (success == ffmpeg.AVERROR(ffmpeg.EAGAIN) || success == ffmpeg.AVERROR(ffmpeg.AVERROR_EOF))
                break;
            else if (success != 0)
                throw new FFMpegException("Frame Decoder: Error receiving frame: success", success);
        }
        return frame;
    }
}

}  // namespace Decoder
}  // namespace Codec

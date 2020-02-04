using System;
using FFmpeg;
using FFmpeg.AutoGen;
using System.Collections.Generic;

namespace Codec {
namespace Encoder {

/// <summary>
/// Class to handle encoding raw data from a Unity texture into h264 video frames
/// </summary>
/// <remarks>
/// It is expected that the input format for the Unity textures is in ARGB32
/// </remarks>
public class FrameEncoder {
    private readonly EncoderContext     encoder_context_;
    private readonly PixelConverter     pixel_conveter_;
    private readonly EncoderFrame       yuv_frame_;
    private readonly int                width_;
    private readonly int                height_;
    private const    AVPixelFormat      SRC_PIXEL_FORMAT = AVPixelFormat.AV_PIX_FMT_RGBA;

    /// <summary>
    /// Construct the frame encoder object
    /// </summary>
    /// <param name="fps">The desired framerate of the stream, in frames per second</param>
    /// <param name="width">The width of each unencoded frame, in pixels</param>
    /// <param name="height">The height of each enencoded frame, in pixels</param>
    public FrameEncoder(int fps, int width, int height)
    {
        width_ = width;
        height_ = height;

        // Set the directory where the DLLs exist
        ffmpeg.RootPath = Environment.CurrentDirectory + "\\Assets\\FFmpeg\\bin\\x86\\";

        encoder_context_ = new EncoderContext(fps, width, height);
        yuv_frame_       = new EncoderFrame(encoder_context_.GetPixelFormat(), width, height);
        pixel_conveter_  = new PixelConverter(
                SRC_PIXEL_FORMAT, encoder_context_.GetPixelFormat(),
                width, height);
    }

    /// <summary>
    /// Encode a single frame
    /// </summary>
    /// <param name="raw_frame">The raw unencoded data frame</param>
    /// <returns>The raw encoded data packet</returns>
    /// <exception cref="System.ArgumentException">
    /// Thrown if the supplied frame is of the wrong length
    /// </exception>
    /// <exception cref="Codec.FFMpegException">
    /// Thrown if the frame was not able to be encoded
    /// </exception>
    public List<Packet> EncodeFrame(byte[] raw_frame)
    {
        // Each frame should be argb32 (4 bytes per pixel)
        if (raw_frame.Length != width_ * height_ * 4)
            throw new ArgumentException(
                    "Error: Invalid Image Size Supplied to Encoder\n" +
                    "Got: " + raw_frame.Length + "\n" +
                    "Expected: " + width_ * height_ * 4);

        yuv_frame_.EnsureWritable();
        AVFrame frame = CodecUtility.RawDataToFrame(raw_frame, SRC_PIXEL_FORMAT, height_);
        yuv_frame_.SetData(frame, pixel_conveter_);
        yuv_frame_.IncrementFrameCount();

        CodecUtility.SendFrame(encoder_context_, yuv_frame_);

        var packets = new List<Packet>();
        int success = 0;
        while (success == 0) {
            var packet = new Packet();
            success = CodecUtility.ReceivePacket(encoder_context_, packet);
            if (success == 0)
                packets.Add(packet);
            else if (success == ffmpeg.AVERROR(ffmpeg.EAGAIN) || success == ffmpeg.AVERROR(ffmpeg.AVERROR_EOF))
                break;
            else
                throw new FFMpegException("Frame Encoder: Could not receive packet: " + success, success);
        }

        return packets;
    }
};

}  // namespace Encoder
}  // namespace Codec

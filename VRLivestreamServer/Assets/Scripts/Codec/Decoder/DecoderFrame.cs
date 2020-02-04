using FFmpeg;
using FFmpeg.AutoGen;

namespace Codec {
namespace Decoder {

/// <summary>
/// Wraps an ffmpeg AVFrame object to be used for decoding purposes
/// </summary>
public unsafe class DecoderFrame {
    private AVFrame* frame_;

    /// <summary>
    /// Construct a new decoder frame
    /// </summary>
    /// <exception cref="Codec.CodecException">
    /// Thrown if frame could not be allocated
    /// </exception>
    public DecoderFrame() {
        frame_ = ffmpeg.av_frame_alloc();
        if (frame_ == null)
            throw new CodecException("Frame: Cannot allocate memory for frame");
    }

    /// <summary>
    /// Get the underlying ffmpeg AVFrame object
    /// </summary>
    /// <returns>The AVFrame object</returns>
    public AVFrame* GetUnmanagedFrame() { return frame_; }

    /// <summary>
    /// Read the width of the frame
    /// </summary>
    /// <returns>The width of the frame</returns>
    public int Width()  { return frame_->width; }

    /// <summary>
    /// Read the height of the frame
    /// </summary>
    /// <returns>The height of the frame</returns>
    public int Height() { return frame_->height; }

    /// <summary>
    /// Read the pixel format of the frame
    /// </summary>
    /// <returns>The pixel format of the frame</returns>
    public AVPixelFormat PixelFormat() { return (AVPixelFormat) frame_->format;}

    ~DecoderFrame() 
    {
        fixed (AVFrame** frame_dptr = &frame_)
            ffmpeg.av_frame_free(frame_dptr);
    }
}

}  // namespace Decoder
}  // namespace Codec

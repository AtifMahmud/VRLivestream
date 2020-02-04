using FFmpeg;
using FFmpeg.AutoGen;

namespace Codec {

/// <summary>
/// Class to handle converting frames between different pixel formats
/// </summary>
public unsafe class PixelConverter {
    private readonly AVPixelFormat src_format_;
    private readonly AVPixelFormat dest_format_;
    private readonly SwsContext*   sws_context_;
    private readonly int           height_;

    /// <summary>
    /// Construct a new pixel converter
    /// </summary>
    /// <param name="src_format">The pixel format of input frames</param>
    /// <param name="dest_format">The pixel format of output frames</param>
    /// <param name="width">The width of each frame, in pixels</param>
    /// <param name="height">The height of each frame, in pixels</param>
    /// <exception cref="Codec.CodecException">
    /// Thrown if the underlying ffmpeg sws_context could not be constructed
    /// </exception>
    public PixelConverter(
            AVPixelFormat src_format, AVPixelFormat dest_format, int width, int height)
    {
        src_format_  = src_format;
        dest_format_ = dest_format;
        height_      = height;

        sws_context_ = ffmpeg.sws_getContext(
                width, height, src_format_,
                width, height, dest_format_, 
                ffmpeg.SWS_FAST_BILINEAR, null, null, null);
    
        if (sws_context_ == null)
            throw new CodecException(
                    "PixelConverter: Cannot instantiate pixel format convertor");
    }

    /// <summary>
    /// Convert a frame from one pixel type to another
    /// </summary>
    /// <param name="src_frame">The source frame</param>
    /// <param name="dest_data">Tne output frame to data</param>
    /// <param name="dest_line_size">The output frame linesize</param>
    public void ConvertPixels(
            AVFrame* src_frame, byte_ptrArray4 dest_data, int_array4 dest_line_size)
    {
        ffmpeg.sws_scale(
                sws_context_, src_frame->data, src_frame->linesize, 0,
                height_, dest_data, dest_line_size);
    }

    /// <summary>
    /// Same as above, but with different output types
    /// </summary>
    /// <param name="src_frame">The source frame</param>
    /// <param name="dest_data">Tne output frame to data</param>
    /// <param name="dest_line_size">The output frame linesize</param>
    public void ConvertPixels(
            AVFrame* src_frame, byte_ptrArray8 dest_data, int_array8 dest_line_size)
    {
        ffmpeg.sws_scale(
                sws_context_, src_frame->data, src_frame->linesize, 0,
                height_, dest_data, dest_line_size);
    }

    /// <summary>
    /// Read the source pixel format
    /// </summary>
    /// <returns>The source pixel format</returns>
    public AVPixelFormat SrcFormat()  { return src_format_; }
    
    /// <summary>
    /// Read the destination pixel format
    /// </summary>
    /// <returns>The desitination pixel format</returns>
    public AVPixelFormat DestFormat() { return dest_format_; }
    
    ~PixelConverter() 
    {
        ffmpeg.sws_freeContext(sws_context_);
    }
}

}  // namespace Codec

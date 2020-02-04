using FFmpeg;
using FFmpeg.AutoGen;

namespace Codec {
namespace Encoder {

/// <summary>
/// Wrapper around an ffmpeg AVFrame that will be encoded
/// </summary>
/// <remarks>
/// Construction of this object will allocate a buffer large enough for one 
/// unencoded video frame which can be resused to encode multiple frames.
/// The intended use of this class is as follows:
/// 1. Get a frame of unencoded data
/// 2. Call EnsureWritable to make sure the underlying AVFrame is writable
/// 3. Call SetData to fill the frame buffer with the unencoded data
/// 4. Call GetUnmanagedFrame when ready to pass the frame to the ffmpeg encoder
/// 5. Increment the frame count
/// 6. Repeat
///  </remarks>
public unsafe class EncoderFrame {
    private AVFrame*               frame_;
    private readonly AVPixelFormat pixel_format_;
    private int                    frame_count_ = 0;

    /// <summary>
    /// Create the Encoder frame Object
    /// </summary>
    /// <param name="pixel_format">The pixel format for the encoded frame</param>
    /// <param name="width">The width of each image frame to be encoded, in pixels</param>
    /// <param name="height">The height of each image frame to be encoded, in pixels</param>
    /// <exception cref="Codec.CodecException">
    /// Thrown if the frame data or the frame buffer could not be allocated
    /// </exception>
    public EncoderFrame(AVPixelFormat pixel_format, int width, int height) {
        pixel_format_ = pixel_format;

        frame_ = ffmpeg.av_frame_alloc();
        if (frame_ == null)
            throw new CodecException("Encoder Frame: Cannot allocate memory for frame");
        
        frame_->format = (int) pixel_format;  
        frame_->width  = width;
        frame_->height = height;
    
        int success = ffmpeg.av_frame_get_buffer(frame_, 0);
        if (success < 0)
            throw new CodecException("Encoder Frame: Cannot allocate memory for frame data");
    }

    /// <summary>
    /// Get a pointer to the underlying ffmpeg AVFrame object
    /// </summary>
    /// <returns>The AVFrame object</returns>
    public AVFrame* GetUnmanagedFrame() { return frame_; }

    /// <summary>
    /// Set data for the frame, given a ffmpeg AVFrame object and a pixel converter
    /// to convert the supplied frame into the correct pixel format
    /// </summary>
    /// <param name="src_frame">The supplied frame</param>
    /// <param name="converter">The converter to convert between pixel formats</param>
    /// <exception cref="Codec.CodecException">
    /// Thrown if the supplied frame does not match the source format of the pixel converter
    /// or the pixel format of the encoder frame (this) does not match the output format 
    /// of the converter 
    /// </exception>
    public void SetData(AVFrame src_frame, PixelConverter converter)
    {
        if (src_frame.format != (int) converter.SrcFormat())
            throw new CodecException("Encoder Frame: Src pixel formats do not match.\n" +
                                     "Expected:  " + src_frame.format  + "\n" + 
                                     "Got: " + (int) converter.SrcFormat());
        if (converter.DestFormat() != pixel_format_)
            throw new CodecException("Encoder Frame: Destination pixel formats do not match.\n" +
                                     "Expected:  " + pixel_format_  + "\n" + 
                                     "Got: " + converter.DestFormat());
        
        converter.ConvertPixels(&src_frame, frame_->data, frame_->linesize);
    }

    /// <summary>
    /// Sets the frame to be writable
    /// </summary>
    /// <remarks>
    /// This should be called before attempting to call the SetData function
    /// </remarks>
    /// <exception cref="Codec.CodecException">
    /// Thrown if the frame could not be made writeable
    /// </exception>
    public void EnsureWritable()
    {
        int success = ffmpeg.av_frame_make_writable(frame_);
        if (success < 0)
            throw new CodecException("Encoder Frame: frame is not writable");
    }

    /// <summary>
    /// Increment the frame count
    /// </summary>
    public void IncrementFrameCount()
    {  
        frame_->pts = frame_count_;
        ++frame_count_;
    }

    ~EncoderFrame() {
        fixed (AVFrame** frame_dptr = &frame_)
            ffmpeg.av_frame_free(frame_dptr);
    }
}

}  // namespace Encoder
}  // namespace Codec

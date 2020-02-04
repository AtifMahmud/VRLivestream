using FFmpeg;
using FFmpeg.AutoGen;

namespace Codec {
namespace Encoder {

/// <summary>
/// Wrapper around an ffmpeg encoder context for the h264 codec
/// </summary>
public unsafe class EncoderContext {
    private readonly AVCodec*         codec_ptr_;
    private          AVCodecContext*  codec_context_ptr_;
    private const    AVPixelFormat    PIX_FMT = AVPixelFormat.AV_PIX_FMT_YUV420P;

    /// <summary>
    /// Construct a new encoder context
    /// </summary>
    /// <param name="fps">The framerate of the stream, in frames per second</param>
    /// <param name="width">The width of each image frame, in pixels</param>
    /// <param name="height">The height of each image frame, in pixels</param>
    /// <exception cref="Codec.CodecException">
    /// Thrown run the encoder context could not be created
    /// </exception>
    /// <exception cref="Codec.FFMpegException">
    /// Thrown run the encoder context could not be opened
    /// </exception>
    public EncoderContext(int fps, int width, int height)
    {
        var codecId = AVCodecID.AV_CODEC_ID_H264;
        codec_ptr_ = ffmpeg.avcodec_find_encoder(codecId);
        if (codec_ptr_ == null) 
            throw new CodecException("Encoder Context: Codec not found.");

        codec_context_ptr_ = ffmpeg.avcodec_alloc_context3(codec_ptr_);
        if (codec_context_ptr_ == null)
            throw new CodecException("Encoder Context: Could not allocate codec context.");
        
        codec_context_ptr_->width = width;
        codec_context_ptr_->height = height;
        codec_context_ptr_->time_base = new AVRational {num = 1, den = fps};
        codec_context_ptr_->framerate = new AVRational {num = fps, den = 1};
        codec_context_ptr_->pix_fmt = PIX_FMT;
        codec_context_ptr_->gop_size = 25;  // number of frames for 1 i-frame
        codec_context_ptr_->max_b_frames = 0;  // No B-Frames, which need future frames
        codec_context_ptr_->codec_type = AVMediaType.AVMEDIA_TYPE_VIDEO;
        ffmpeg.av_opt_set(codec_context_ptr_->priv_data, "preset", "ultrafast", 0);

        int success = ffmpeg.avcodec_open2(codec_context_ptr_, codec_ptr_, null);
        if (success < 0)
            throw new FFMpegException("Encoder Context: Could not open encoder context", success);
    }

    /// <summary>
    /// Get the pixel format of the encoder context
    /// </summary>
    /// <remarks>
    /// Tne pixel format of the encoder will match the pixel format of each encoded video frame
    /// </remarks>
    /// <returns>The pixel format</returns>
    public AVPixelFormat GetPixelFormat() { return codec_context_ptr_->pix_fmt; }

    /// <summary>
    /// Get the underlying ffmpeg AVCodecContext object
    /// </summary>
    /// <returns>The encoder context</returns>
    public AVCodecContext* GetContext() { return codec_context_ptr_; }

    /// <summary>
    /// Get the underlying ffmpeg AVCode object
    /// </summary>
    /// <returns>The codec</returns>
    public AVCodec* GetCodec() { return codec_ptr_; }

    ~EncoderContext()
    {
        ffmpeg.avcodec_close(codec_context_ptr_);
        ffmpeg.av_free(codec_context_ptr_);
        ffmpeg.av_free(codec_ptr_);
    }    
}

}  // namespace Encoder
}  // namespace Codec

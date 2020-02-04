using FFmpeg;
using FFmpeg.AutoGen;

namespace Codec {
namespace Decoder {

/// <summary>
/// Wraps an ffmpeg decoder context for an h264 decoder
/// </summary>
public unsafe class DecoderContext {
    private readonly AVCodec*         codec_ptr_;
    private          AVCodecContext*  codec_context_ptr_;

    /// <summary>
    /// Constrycts a new Decoder
    /// </summary>
    /// <exception cref="Codec.CodecException">
    /// Thrown if decocder could not be found or allocated
    /// </exception>
    /// <exception cref="Codec.FFMpegException">
    /// Thrown if the decoder context could not be opened
    /// </exception>
    public DecoderContext()
    {
        // Find the H264 Encoder
        var codecId = AVCodecID.AV_CODEC_ID_H264;
        codec_ptr_ = ffmpeg.avcodec_find_decoder(codecId);
        if (codec_ptr_ == null) 
            throw new CodecException("Decoder Context: Codec not found.");

        codec_context_ptr_ = ffmpeg.avcodec_alloc_context3(codec_ptr_);
        if (codec_context_ptr_ == null)
          throw new CodecException("Decoder Context:: Could not allocate codec context.");

        int success = ffmpeg.avcodec_open2(codec_context_ptr_, codec_ptr_, null);
        if (success < 0)
            throw new FFMpegException("Decoder Context: Could not open codec", success);
    }

    /// <summary>
    /// Get a pointer to the ffmpeg decoder context
    /// </summary>
    /// <returns>The decoder context</returns>
    public AVCodecContext* GetContext() { return codec_context_ptr_; }

    ~DecoderContext()
    {
        ffmpeg.avcodec_close(codec_context_ptr_);
        ffmpeg.av_free(codec_context_ptr_);
        ffmpeg.av_free(codec_ptr_);
    }
}

}  // namespace Decoder
}  // namespace Codec
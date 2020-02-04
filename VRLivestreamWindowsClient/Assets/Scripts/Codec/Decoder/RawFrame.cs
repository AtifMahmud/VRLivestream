using System;
using System.Runtime.InteropServices;
using FFmpeg;
using FFmpeg.AutoGen;

namespace Codec {
namespace Decoder {

/// <summary>
/// Class to wrap a raw frame data outside of an ffmpeg AVFrame object
/// </summary>
public unsafe class RawFrame {
    private          IntPtr          buffer_ptr_;
    private          byte_ptrArray4  data_;
    private          int_array4      line_size_;
    private readonly int             buffer_size_;
    private readonly AVPixelFormat   pixel_format_;

    /// <summary>
    /// Construct the RawFrame object
    /// </summary>
    /// <param name="pixel_format">The pixel format of the frame</param>
    /// <param name="width">The width of the frame, in pixels</param>
    /// <param name="height">The height of the frame, in pixels</param>
    /// <exception cref="Codec.FFMpegException">\
    /// Thrown if the frame buffer could not be allocated
    /// </exception>
    public RawFrame(
            AVPixelFormat pixel_format, int width, int height)
    {
        pixel_format_ = pixel_format;
        buffer_size_ = ffmpeg.av_image_get_buffer_size(
                pixel_format, width, height, 1);

        buffer_ptr_ = Marshal.AllocHGlobal(buffer_size_);
        data_ = new byte_ptrArray4();
        line_size_ = new int_array4();

        int success = ffmpeg.av_image_fill_arrays(
                ref data_, ref line_size_,
                (byte*) buffer_ptr_,
                pixel_format,
                width, height, 1);

        if (success < 0)
            throw new FFMpegException(
                    "Raw Frame: Could not allocate raw frame buffer", success);
    }

    /// <summary>
    /// Set the data for the raw frame given an ffmpeg AVFrame object
    /// </summary>
    /// <param name="frame">The ffmpeg AVFrame object</param>
    /// <param name="converter">The converter to convert between pixel formats</param>
    /// <exception cref="Codec.CodecException">
    /// Thrown in the case of mismatch between frame pixel formats and the converter
    /// pixel format
    public void SetData(DecoderFrame frame, PixelConverter converter)
    {
        if (frame.PixelFormat() != converter.SrcFormat() || converter.DestFormat() != pixel_format_)
            throw new CodecException("Frame Error: Pixel Formats do not match: " +
                                     "Expected:  " + pixel_format_  +
                                     "Got: " + converter.DestFormat());
        converter.ConvertPixels(frame.GetUnmanagedFrame(), data_, line_size_);
    }

    /// <summary>
    /// Copies the raw frame data into a c# managed memory array
    /// </summary>
    /// <returns>The byte array of managed memory</returns>
    public byte[] CopyToManagedMem()
    {
        byte[] managed_data = new byte[buffer_size_];
        Marshal.Copy((IntPtr)data_[0], managed_data, 0, buffer_size_);
        return managed_data;
    }

    ~RawFrame()
    {
        Marshal.FreeHGlobal(buffer_ptr_);
    }
}

}  // namespace Decoder
}  // namespace Codec

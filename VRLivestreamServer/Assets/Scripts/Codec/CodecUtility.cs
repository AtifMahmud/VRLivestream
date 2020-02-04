using System;
using FFmpeg;
using FFmpeg.AutoGen;
using Codec.Decoder;
using Codec.Encoder;

namespace Codec {

/// <summary>
/// Class that contains all static utility functions related to encoding/decoding
/// </summary>
public static class CodecUtility {
    /// <summary>
    /// Converts a raw byte array representing an unencoded image frame
    /// into an ffmpeg AVFrame object
    /// </summary>
    /// <param name="frame_data">The raw frame data</param>
    /// <param name="format">The pixel format of the raw data</param>
    /// <param name="frame_height">The height of the image frame in pixels</param>
    /// <returns></returns>
    public unsafe static AVFrame RawDataToFrame(byte[] frame_data, AVPixelFormat format, int frame_height)
    {
        // Unity uses bottom-up (start from bottom-left) images instead of the normal
        // top-down images (start from top-right) so flip the array
        // This isn't necessary if both the server and the client are implemented in Unity
        // If we remove this, we'll need to remove it in the decoder as well.
        byte[] flipped_frame_data = GetFlippedArray(
                frame_data, frame_height, frame_data.Length / frame_height);

        fixed (byte* raw_frame_ptr = flipped_frame_data)
        {
            var data = new byte_ptrArray8 { [0] = raw_frame_ptr };
            var linesize = new int_array8 { [0] = frame_data.Length / frame_height };
            var frame = new AVFrame
            {
                data = data,
                linesize = linesize,
                height = frame_height,
                format = (int) format
            };
            return frame;
        }
    }

    /// <summary>
    /// Send a frame to the encoder
    /// </summary>
    /// <param name="encoder_context">The encoder context to send the frame to</param>
    /// <param name="yuv_frame">The frame to encode</param>
    /// <exception cref="Codec.FFMpegException">
    /// Thrown if the frame could not be sent to the encoder
    /// </exception>
    public unsafe static void SendFrame(EncoderContext encoder_context, EncoderFrame yuv_frame)
    {
        int success = ffmpeg.avcodec_send_frame(
        encoder_context.GetContext(), yuv_frame.GetUnmanagedFrame());

        if (success != 0)
            throw new FFMpegException("Error sending frame for encoding", success);
    }

    /// <summary>
    /// Receive an encoded packet from the encoder
    /// </summary>
    /// <param name="encoder_context">The encoder context to receive the frame from</param>
    /// <param name="packet">The packet to update</param>
    /// <returns>The return flag from the underlying ffmpeg avcodec_receive_packet function</returns>
    public unsafe static int ReceivePacket(EncoderContext encoder_context, Packet packet)
    {
        return ffmpeg.avcodec_receive_packet(
                encoder_context.GetContext(),
                packet.GetUnmanagedPacket());
    }

    /// <summary>
    /// Send a packet to the decoder for decoding
    /// </summary>
    /// <param name="decoder_context">The decoder context to send the packet to</param>
    /// <param name="packet">The packet to decode</param>
    /// <exception cref="Codec.FFMpegException">
    /// Thrown if the packet could not be sent to the decoder
    /// </exception>
    public unsafe static void SendPacket(DecoderContext decoder_context, Packet packet)
    {
        int success = ffmpeg.avcodec_send_packet(decoder_context.GetContext(), packet.GetUnmanagedPacket());
        if (success != 0)
            throw new FFMpegException("Error sending packet for decoding", success);
    }

    /// <summary>
    /// Receive an unencoded frame from the decoder
    /// </summary>
    /// <param name="decoder_context">The decoder context to get the frame from</param>
    /// <param name="yuv_frame">The decoder frame to update</param>
    /// <returns>The return val from the underlying ffmpeg avcodec_receive_frame function</returns>
    public unsafe static int ReceiveFrame(DecoderContext decoder_context, DecoderFrame yuv_frame)
    {
        return ffmpeg.avcodec_receive_frame(decoder_context.GetContext(), yuv_frame.GetUnmanagedFrame());
    }

    /// <summary>
    /// Flips an image array
    /// If the input array is top-down the output array will be bottom-up and vice versa
    /// </summary>
    /// <param name="arr"> The input array to flip</param>
    /// <param name="num_rows">The number of rows in the array</param>
    /// <param name="num_cols">The number of columns in the array</param>
    /// <returns>The flipped array</returns>
    /// <exception cref="System.ArgumentException">
    /// Thrown if the input array size does not equal the number of rows * number of cols
    /// </exception>
    public static byte[] GetFlippedArray(byte[] arr, int num_rows, int num_cols)
    {
        //if (num_rows * num_cols != arr.Length)
        //    throw new ArgumentException(
        //            "Error when flipping array: " +
        //            "Array length does not equal the number of rows by the number of columns");

        //var flipped_arr = new byte[arr.Length];
        //for (int row = 0; row < num_rows; ++row)
        //    for (int col = 0; col < num_cols; ++col)
        //        flipped_arr[row * num_cols + col] = arr[(num_rows - row - 1) * num_cols + col];
        //return flipped_arr;
        return arr;
    }
}

}  // namespace Codec

using System;
using System.Runtime.InteropServices;
using FFmpeg;
using FFmpeg.AutoGen;

namespace Codec {

/// <summary>
/// Wrapper for the ffmpeg AVPacket class
/// </summary>
public unsafe class Packet {
    private AVPacket* packet_;

    /// <summary>
    /// Construct a new Packet
    /// </summary>
    public Packet()
    {
        // Allocate memory for an encoded packet
        packet_ = ffmpeg.av_packet_alloc();
        if (packet_ == null)
            throw new CodecException("Packet: Cannot allocate memory for packet");
    }

    /// <summary>
    /// Construct a new packet given raw packet data
    /// </summary>
    /// <param name="raw_data">The packet data</param>
    /// <remarks>
    /// This constructor can be called using the results of the GetRawPacketData function
    /// </remarks>
    public Packet(byte[] raw_data)
    {
        packet_ = ffmpeg.av_packet_alloc();
        if (packet_ == null)
            throw new CodecException("Packet: Cannot allocate memory for packet");

        packet_->data = (byte*) Marshal.AllocHGlobal(raw_data.Length);
        Marshal.Copy(raw_data, 0, (IntPtr) packet_->data, raw_data.Length);
        packet_->size = raw_data.Length;
    }

    /// <summary>
    /// Retrieve the underlying ffmpeg AVPacket object
    /// </summary>
    /// <returns></returns>
    public AVPacket* GetUnmanagedPacket() { return packet_; }

    /// <summary>
    /// Get the size of the encoded video packet
    /// </summary>
    /// <returns>The size of the packet</returns>
    public int Size() { return packet_->size; }

    /// <summary>
    /// Convert an ffmpeg packet into a byte array that can be send over the network
    /// </summary>
    /// <param name="packet">The packet</param>
    /// <returns>A byte array of the packet data</returns>
    public byte[] GetRawPacketData()
    {
        byte[] managed_data = new byte[packet_->size];
        Marshal.Copy((IntPtr)packet_->data, managed_data, 0, packet_->size);
        return managed_data;
    }

    ~Packet()
    {
        ffmpeg.av_packet_unref(packet_);
        fixed (AVPacket** packet_dptr = &packet_)
            ffmpeg.av_packet_free(packet_dptr);
    }
}

}  // namespace Codec

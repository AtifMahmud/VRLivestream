using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using UnityEngine;

namespace Codec {
namespace Decoder {

public unsafe class FrameDecoderWrapper {
    private IntPtr frame_decoder_ = IntPtr.Zero;
    private List<byte[]> decoded_data_;
    private const int MAX_FRAMES = 5;

    public FrameDecoderWrapper()
    {
        decoded_data_ = new List<byte[]>();
        frame_decoder_ = CreateDecoder();
        
        int buffer_size = GetBufferSize(
            frame_decoder_, 
            Settings.img_width_, 
            Settings.img_height_);
        
        if (buffer_size <= 0) {
            Debug.Log("Error, could not get buffer size!");
            DestroyDecoder(frame_decoder_);
            frame_decoder_ = IntPtr.Zero;
        }

        for (int i = 0; i < MAX_FRAMES; ++ i) {
            var arr = new byte[buffer_size];
            decoded_data_.Add(arr);
            int success = AddBuffer(
              frame_decoder_, 
              arr,
              Settings.img_width_, 
              Settings.img_height_);

            if (success < 0) {
                UnityEngine.Debug.Log("Error, could not set buffers!");
                DestroyDecoder(frame_decoder_);
                frame_decoder_ = IntPtr.Zero;
            }
        }
    }

    ~FrameDecoderWrapper()
    {
        DestroyDecoder(frame_decoder_);
    }

    public List<byte[]> DecodeFrame(byte[] data)
    {
        int num_received = DecodeFrame(
            frame_decoder_, 
            data, 
            data.Length);

        if (num_received > 0)
            return decoded_data_.GetRange(0, num_received);
        else 
            return new List<byte[]>();
    }

    [DllImport("libdecoder")]
    private static extern IntPtr CreateDecoder();

    [DllImport("libdecoder")]
    private static extern void DestroyDecoder(IntPtr decoder);

    [DllImport("libdecoder")]
    private static extern int DecodeFrame(
        IntPtr decoder, 
        byte[] raw_data, 
        int raw_length);

    [DllImport("libdecoder")]
    private static extern int GetBufferSize(
        IntPtr decoder,
        int width,
        int height);

    [DllImport("libdecoder")]
    private static extern int AddBuffer(
        IntPtr decoder, 
        byte[] buffer,
        int width,
        int height);
}

}  // namespace Decoder
}  // namespace Codec

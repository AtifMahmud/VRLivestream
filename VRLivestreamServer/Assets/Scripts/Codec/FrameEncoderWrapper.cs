using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using UnityEngine;

namespace Codec {
namespace Encoder {

public unsafe class FrameEncoderWrapper {
    private IntPtr frame_encoder_ = IntPtr.Zero;
    private const int MAX_PACKETS = 5;

    public FrameEncoderWrapper(int fps, int width, int height)
    {
        frame_encoder_ = CreateEncoder(fps, width, height, MAX_PACKETS);
    }

    ~FrameEncoderWrapper()
    {
        DestroyEncoder(frame_encoder_);
    }

    public List<byte[]> EncodeFrame(byte[] data)
    {
        int[] encoded_length = new int[MAX_PACKETS];
        byte*[] encoded_data = new byte*[MAX_PACKETS];
        
        int num_received_packets = EncodeFrame(
            frame_encoder_, 
            data, 
            data.Length, 
            encoded_data,
            encoded_length);

        if (num_received_packets > 0) {
          List<byte[]> packets = new List<byte[]>();
          for (int i = 0; i < num_received_packets; ++i) {
              byte[] managed_arr = new byte[encoded_length[i]];
              System.Runtime.InteropServices.Marshal.Copy(
                  (IntPtr)encoded_data[i], managed_arr, 0, encoded_length[i]);
              packets.Add(managed_arr);
          }
          return packets;
        } else {
          return new List<byte[]>();
        }
    }

    [DllImport("libencoder")]
    private static extern IntPtr CreateEncoder(
        int fps, int width, int height, int max_packets);

    [DllImport("libencoder")]
    private static extern void DestroyEncoder(IntPtr encoder);

    [DllImport("libencoder")]
    private static extern int EncodeFrame(
        IntPtr encoder, 
        byte[] raw_data, 
        int raw_length,
        byte*[] encoded_data,
        int[] encoded_length);
}

}  // namespace Encoder
}  // namespace Codec

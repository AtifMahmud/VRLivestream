using UnityEngine;
using UnityEngine.Rendering;
using System;
using FS;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using Codec;
using Codec.Decoder;
using Codec.Encoder;
using FFmpeg;
using FFmpeg.AutoGen;

[RequireComponent(typeof(Camera))]
public class Test360Camera : MonoBehaviour
{
    public  const int FRAMERATE = 10;
    private const int DIM = 1024;
    private Camera cam_;
    private RenderTexture cubemap_left_;
    private RenderTexture equirect_;
    private Texture2D display_texture_;
    private Queue<AsyncGPUReadbackRequest> async_requests_ = new Queue<AsyncGPUReadbackRequest>();
    private int max_requests_ = 10;
    private FrameEncoder encoder_;
    private FrameDecoder decoder_;
    private FrameServer frameserver_;

    void Start()
    {
        frameserver_ = new FrameServer();
        cam_ = GetComponent<Camera>();
        cubemap_left_ = new RenderTexture(DIM, DIM, 24, RenderTextureFormat.ARGB32);
        cubemap_left_.dimension = TextureDimension.Cube;
        equirect_ = new RenderTexture(DIM, DIM, 24, RenderTextureFormat.ARGB32);
        display_texture_ = new Texture2D(DIM, DIM, TextureFormat.RGBA32, false);
        try {
            encoder_  = new FrameEncoder(FRAMERATE, DIM, DIM);
            decoder_  = new FrameDecoder();
        } catch (FFMpegException e) {
            Debug.LogError(
                    "Could not construct video encoder or decoder: " + 
                    e.Message + 
                    " - FFMpeg error code: " 
                    + e.ErrCode());
        } 
        catch (CodecException e) {
            Debug.LogError("Could not construct video encoder or decoder: " + e.Message);
        }
    }

    void Update() {
        // TODO Move this out of the update (a separate thread with a queue?)
        // Check to see if the Async request is finished.
        // If first item in queue not ready, don't look at the rest (so that images stay in order)
        while (async_requests_.Count > 0) {
            var req = async_requests_.Peek();
            if (req.hasError) {
                Debug.LogWarning("Error during async GPU handle!");
                async_requests_.Dequeue();
                continue;
            }
            if (req.done) {
                var img = req.GetData<byte>();
                try {
                    List<Packet> packets = encoder_.EncodeFrame(img.ToArray());

                    foreach(var packet in packets) {
                        SendPacket(packet);
                    }
                } catch (Codec.FFMpegException e) {
                    Debug.LogError(
                            "Caught error during encoding/decoding process: " + e.Message +
                            "\nError code: " + e.ErrCode());
                } catch (Codec.CodecException e) {
                    Debug.LogError(
                            "Caught error during encoding/decoding process: " + e.Message);
                }
                async_requests_.Dequeue();
            }
            else {
                break;
            }
        }
    }

    unsafe void SendPacket(Packet packet) {
        byte[] arr = packet.GetRawPacketData();

        frameserver_.SendFrame(arr);
    }

    void LateUpdate() {
        // 63 = 0b111111 Which indicates to render all 6 faces of the cubemap
        cam_.RenderToCubemap(cubemap_left_, 63, Camera.MonoOrStereoscopicEye.Mono);
        cubemap_left_.ConvertToEquirect(equirect_, Camera.MonoOrStereoscopicEye.Mono);

        // Get the texture asynchronously - this avoids GPU/CPU pipeline stalls
        if (async_requests_.Count < max_requests_) {
            async_requests_.Enqueue(UnityEngine.Rendering.AsyncGPUReadback.Request(equirect_));
        } else {
            Debug.LogWarning("Reached Limit of Requests!");
        }
    }
    
    void OnApplicationQuit()
    {
        frameserver_.Cleanup();
    }
}

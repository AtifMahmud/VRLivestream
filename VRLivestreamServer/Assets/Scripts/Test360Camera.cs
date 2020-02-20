using UnityEngine;
using UnityEngine.Rendering;
using System;
using FS;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using Codec.Encoder;

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
    private FrameEncoderWrapper encoder_;
    private FrameServer frameserver_;

    void Start()
    {
        frameserver_ = new FrameServer();
        cam_ = GetComponent<Camera>();
        cubemap_left_ = new RenderTexture(DIM, DIM, 24, RenderTextureFormat.ARGB32);
        cubemap_left_.dimension = TextureDimension.Cube;
        equirect_ = new RenderTexture(DIM, DIM, 24, RenderTextureFormat.ARGB32);
        display_texture_ = new Texture2D(DIM, DIM, TextureFormat.RGBA32, false);
        encoder_  = new FrameEncoderWrapper(FRAMERATE, DIM, DIM);
        if (encoder_ == null) {
            Debug.Log("ERROR failed to create encoder");
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
                    List<byte[]> packets = encoder_.EncodeFrame(img.ToArray());

                    foreach(var packet in packets) {
                        frameserver_.SendFrame(packet);
                    }
                async_requests_.Dequeue();
            }
            else {
                break;
            }
        }
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

using Server;
using System;
using System.Threading.Tasks;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using UnityEngine;
using UnityEngine.Rendering;

/// <summary>
/// This is the main C# component of the server-side application
/// It should be attached to the main camera in the scene
/// This component will create an RTP streaming server.
/// On every frame update, this component will attempt to extract a 360 degree
/// frame from Unity and send it over RTP to connected clients
/// </summary>
[RequireComponent(typeof(Camera))]
public class ClientCamera : MonoBehaviour
{
    public  const int FRAMERATE = 30;
    private const int DIM = 1024;
    private const int MAX_REQUESTS = 10;

    private Camera cam_;
    private RenderTexture cubemap_left_;
    private RenderTexture equirect_;
    private Texture2D texture_;

    private Queue<AsyncGPUReadbackRequest> async_requests_ = new Queue<AsyncGPUReadbackRequest>();
    private Server.RTPStreamAsyncHandler stream_task_;
    private Server.SdpSender sdp_server_;

    private bool supports_async_gpu_;

    /// <summary>
    /// Creates all the underlying components
    /// </summary>
    void Start()
    {
        cam_ = GetComponent<Camera>();

        cubemap_left_ = new RenderTexture(DIM, DIM, 24, RenderTextureFormat.ARGB32);
        cubemap_left_.dimension = TextureDimension.Cube;
        equirect_ = new RenderTexture(DIM, DIM, 24, RenderTextureFormat.ARGB32);

        // The encode task handles sending frames to the rtp stream in a semi-async fashion
        stream_task_  = new Server.RTPStreamAsyncHandler(FRAMERATE, DIM, DIM);
        if (stream_task_ == null)
            Debug.LogError("ERROR failed to create streaming task");

        // Async GPU callbacks prevent the CPU from being stalled when GPU tasks
        // are running. The performance is significantly better on machines
        // that support this functionality
        supports_async_gpu_ = UnityEngine.SystemInfo.supportsAsyncGPUReadback;
        if (!supports_async_gpu_) {
            texture_ = new Texture2D(DIM, DIM, TextureFormat.ARGB32, false);
            UnityEngine.Debug.LogWarning(
                "Device does not support asynchronous GPU Callbacks in Unity. " +
                "Performance will suffer as a result.");
        }

        // Start a TCP server that serves SDP files to clients that want to
        // connect to the RTP stream
        sdp_server_ = new Server.SdpSender(stream_task_);
        Task.Run(() => AcceptRequests());
    }

    /// <summary>
    /// Extract the 360 video frame from Unity for the current frame
    /// </summary>
    void LateUpdate()
    {
        if (supports_async_gpu_) {
            CheckAsyncQueue();
            AddToAsyncQueue();
        } else {
            SyncRender();
        }
    }

    /// <summary>
    /// Extracts the 360 video frame from Unity in a synchronous fashion
    /// and sends it to the RTP stream.
    /// This is used if async gpu callbacks are not supported and the performance
    /// is significantly worse than the async version
    /// </summary>
    void SyncRender()
    {
        // 63 = 0b111111 Which indicates to render all 6 faces of the cubemap
        cam_.RenderToCubemap(cubemap_left_, 63, Camera.MonoOrStereoscopicEye.Mono);
        cubemap_left_.ConvertToEquirect(equirect_, Camera.MonoOrStereoscopicEye.Mono);
        RenderTexture.active = equirect_;
        texture_.ReadPixels(new Rect(0, 0, equirect_.width, equirect_.height), 0, 0);
        byte[] img = texture_.GetRawTextureData();
        stream_task_.SendFrameAsync(img);
    }

    /// <summary>
    /// Checks the queue of gpu callbacks. For any requests that are ready, the
    /// frame will be sent to the RTP stream.
    /// </summary>
    void CheckAsyncQueue()
    {
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
                stream_task_.SendFrameAsync(img.ToArray());
                async_requests_.Dequeue();
            }
            else {
                break;
            }
        }
    }

    /// <summary>
    /// Adds a gpu callback to the queue of callbacks.
    /// The gpu callback will hold the extracted 360 degree video frame
    /// for the current frame
    /// </summary>
    void AddToAsyncQueue()
    {
        // 63 = 0b111111 Which indicates to render all 6 faces of the cubemap
        cam_.RenderToCubemap(cubemap_left_, 63, Camera.MonoOrStereoscopicEye.Mono);
        cubemap_left_.ConvertToEquirect(equirect_, Camera.MonoOrStereoscopicEye.Mono);

        // Get the texture asynchronously - this avoids GPU/CPU pipeline stalls
        if (async_requests_.Count < MAX_REQUESTS) {
            async_requests_.Enqueue(UnityEngine.Rendering.AsyncGPUReadback.Request(equirect_));
        } else {
            Debug.LogWarning("Reached Limit of Requests!");
        }
    }

    /// <summary>
    /// Creates an endless loop to accept client requests to connect to the server
    /// </summary>
    void AcceptRequests()
    {
        while (true)
            sdp_server_.AcceptRequests();
    }
}

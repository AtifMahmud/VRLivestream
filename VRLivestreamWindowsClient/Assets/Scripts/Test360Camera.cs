using UnityEngine;
using UnityEngine.Rendering;
using FS;
using Codec;
using Codec.Decoder;
using FFmpeg;
using FFmpeg.AutoGen;

public class Test360Camera : MonoBehaviour
{
    private RenderTexture cubemapLeftEye;
    private RenderTexture equirect;
    private Texture2D newTexture;
    public float stereoSeparation = 0.064f;
    private float time;
    private const int DIM = 1024;
    Texture2D texture;
    byte[] array;

    private FrameClient frameclient_;
    private FrameDecoder decoder_;

    public Material m;

    // Start is called before the first frame update
    void Start()
    {
        texture = new Texture2D(DIM, DIM, TextureFormat.ARGB32, false);
        cubemapLeftEye = new RenderTexture(DIM, DIM, 24, RenderTextureFormat.ARGB32);
        cubemapLeftEye.dimension = TextureDimension.Cube;
        //equirect height should be twice the height of cubemap
        equirect = new RenderTexture(DIM, DIM, 24, RenderTextureFormat.ARGB32);

        newTexture = new Texture2D(DIM, DIM, TextureFormat.RGBA32, false);
        texture = new Texture2D(DIM, DIM, TextureFormat.ARGB32, false);

        frameclient_ = new FrameClient();
        try {
            decoder_  = new FrameDecoder();
        } catch (FFMpegException e) {
            Debug.LogError(
                    "Could not construct video decoder: " + 
                    e.Message + 
                    " - FFMpeg error code: " 
                    + e.ErrCode());
        } 
        catch (CodecException e) {
            Debug.LogError("Could not construct video decoder: " + e.Message);
        }
    }

    void LateUpdate()
    {
        try {
            byte[] frame = TryReceiveFrame();

            if (frame != null) {
                Debug.Log("decoded frame size " + frame.Length);
                newTexture.LoadRawTextureData(frame);
                newTexture.Apply();
                m.SetTexture("_MainTex", newTexture);
            }
        } catch (Codec.FFMpegException e) {
            Debug.LogError(
                    "Caught error during encoding/decoding process: " + e.Message +
                    "\nError code: " + e.ErrCode());
        } catch (Codec.CodecException e) {
            Debug.LogError(
                    "Caught error during encoding/decoding process: " + e.Message);
        }
    }

    unsafe byte[] TryReceiveFrame() {
            byte[] encoded_frame;
            if (frameclient_.TryReceiveFrame(out encoded_frame)) {
                Packet packet = new Packet(encoded_frame);
                return decoder_.DecodeFrame(packet);
            } else {
                return null;
            }
    }
    
    void OnApplicationQuit()
    {
        frameclient_.Cleanup();
    }
}

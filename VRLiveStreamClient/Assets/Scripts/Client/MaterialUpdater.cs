using UnityEngine;
using UnityEngine.Rendering;
using FS;
using Codec;
using Codec.Decoder;

public class MaterialUpdater : MonoBehaviour
{
    private Texture2D newTexture;
    public float stereoSeparation = 0.064f;
    private float time;
    private const int DIM = Settings.img_width_;
    byte[] array;

    private FrameClient frameclient_;
    private FrameDecoderWrapper decoder_;

    public Material m;

    // Start is called before the first frame update
    void Start()
    {
        newTexture = new Texture2D(DIM, DIM, TextureFormat.RGBA32, false);

        frameclient_ = new FrameClient();
        decoder_  = new FrameDecoderWrapper();
    }

    void LateUpdate()
    {
        byte[] frame = TryReceiveFrame();
        if (frame != null) {
            Debug.Log("decoded frame size " + frame.Length);
            newTexture.LoadRawTextureData(frame);
            newTexture.Apply();
            m.SetTexture("_MainTex", newTexture);
        }
    }

    unsafe byte[] TryReceiveFrame() {
            Debug.Log("waiting for frame...");
            byte[] encoded_frame = frameclient_.TryReceiveFrame();
            if (encoded_frame != null) {
                Debug.Log("OK got it " + encoded_frame.Length);
                return decoder_.DecodeFrame(encoded_frame)[0];
            } else {
                return null;
            }
    }
    
    void OnApplicationQuit()
    {
        frameclient_.Cleanup();
    }
}

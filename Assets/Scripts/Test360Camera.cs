using UnityEngine;
using UnityEngine.Rendering;
using FrameServer;

public class Test360Camera : MonoBehaviour
{
    private FrameServer.FrameServer frameServer;
    private RenderTexture cubemapLeftEye;
    private RenderTexture equirect;
    private Texture2D newTexture;
    public float stereoSeparation = 0.064f;
    private float time;
    private const int DIM = 1024;
    Texture2D texture;
    byte[] array;

    public Material m;

    // Start is called before the first frame update
    void Start()
    {
        frameServer = new FrameServer.FrameServer();
        texture = new Texture2D(DIM, DIM, TextureFormat.ARGB32, false);
        cubemapLeftEye = new RenderTexture(DIM, DIM, 24, RenderTextureFormat.ARGB32);
        cubemapLeftEye.dimension = TextureDimension.Cube;
        //equirect height should be twice the height of cubemap
        equirect = new RenderTexture(DIM, DIM, 24, RenderTextureFormat.ARGB32);

        newTexture = new Texture2D(DIM, DIM, TextureFormat.ARGB32, false);
        texture = new Texture2D(DIM, DIM, TextureFormat.ARGB32, false);
    }

    void LateUpdate()
    {
        Camera cam = GetComponent<Camera>();

        if (cam == null)
        {
            cam = GetComponentInParent<Camera>();
        }

        if (cam == null)
        {
            Debug.Log("stereo 360 capture node has no camera or parent camera");
        }

        cam.RenderToCubemap(cubemapLeftEye, 63, Camera.MonoOrStereoscopicEye.Mono);

        //optional: convert cubemaps to equirect

        if (equirect == null)
            return;

        cubemapLeftEye.ConvertToEquirect(equirect, Camera.MonoOrStereoscopicEye.Mono);

        /** Take render texture to texture 2d and then to byte **/
        RenderTexture.active = equirect;
        texture.ReadPixels(new Rect(0, 0, equirect.width, equirect.height), 0, 0);
        byte[] img = texture.GetRawTextureData(); 
        frameServer.SendFrame(img);
        newTexture.LoadRawTextureData(img);
        newTexture.Apply();
        m.SetTexture("_MainTex", newTexture);
    }

    void OnApplicationQuit()
    {
        frameServer.Cleanup();
    }
    
}

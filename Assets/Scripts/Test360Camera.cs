using UnityEngine;
using UnityEngine.Rendering;

public class Test360Camera : MonoBehaviour
{

    private RenderTexture cubemapLeftEye;
    private RenderTexture cubemapRightEye;
    private RenderTexture equirect;
    public bool renderStereo = true;
    private Texture2D newTexture;
    public float stereoSeparation = 0.064f;
    private float time;
    Texture2D texture;
    byte[] array;

    public Material m;

    // Start is called before the first frame update
    void Start()
    {
        texture = new Texture2D(2048, 2048, TextureFormat.ARGB32, false);
        cubemapLeftEye = new RenderTexture(2048, 2048, 24, RenderTextureFormat.ARGB32);
        cubemapLeftEye.dimension = TextureDimension.Cube;
        cubemapRightEye = new RenderTexture(2048, 2048, 24, RenderTextureFormat.ARGB32);
        cubemapRightEye.dimension = TextureDimension.Cube;
        //equirect height should be twice the height of cubemap
        equirect = new RenderTexture(2048, 2048*2, 24, RenderTextureFormat.ARGB32);

        newTexture = new Texture2D(2048, 2048, TextureFormat.ARGB32, false);
        texture = new Texture2D(2048, 2048, TextureFormat.ARGB32, false);
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

        if (renderStereo)
        {
            cam.stereoSeparation = stereoSeparation;
            cam.RenderToCubemap(cubemapLeftEye, 63, Camera.MonoOrStereoscopicEye.Left);
            cam.RenderToCubemap(cubemapRightEye, 63, Camera.MonoOrStereoscopicEye.Right);
        }
        else
        {
            cam.RenderToCubemap(cubemapLeftEye, 63, Camera.MonoOrStereoscopicEye.Mono);
        }

        //optional: convert cubemaps to equirect

        if (equirect == null)
            return;

        if (renderStereo)
        {
            cubemapLeftEye.ConvertToEquirect(equirect, Camera.MonoOrStereoscopicEye.Left);
            cubemapRightEye.ConvertToEquirect(equirect, Camera.MonoOrStereoscopicEye.Right);
        }
        else
        {
            cubemapLeftEye.ConvertToEquirect(equirect, Camera.MonoOrStereoscopicEye.Mono);
        }



        /** Take render texture to texture 2d and then to byte **/
        //texture = toTexture2D(equirect);
        RenderTexture.active = equirect;
        texture.ReadPixels(new Rect(0, 0, equirect.width, equirect.height), 0, 0);
        //texture.Apply();
        byte[] img = texture.GetRawTextureData(); 
        //array = texture.EncodeToPNG();
        newTexture.LoadRawTextureData(img);
        newTexture.Apply();
        m.SetTexture("_MainTex", newTexture);
    }

}
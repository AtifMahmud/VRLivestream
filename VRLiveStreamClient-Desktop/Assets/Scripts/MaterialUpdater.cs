using UnityEngine;
using UnityEngine.SceneManagement;
using UnityEngine.Rendering;
using System.Collections.Generic;

/// <summary>
/// Main class of the client side
/// This should be attached to the main camera in the scene (or any other component)
/// On every call to update, this class will attempt to load a new frame from the
/// RTP stream and display it
/// </summary>
public class MaterialUpdater : MonoBehaviour
{
    private Client.RTPClientWrapper rtp_client_;
    private Texture2D display_texture_;

    /// <summary>
    /// Public material. Should be set to the material of the sphere
    /// </summary>
    public Material m;

    /// <summary>
    /// Attempts to connect to the server's RTP stream
    /// If it fails to do so, it will reload the main scene
    /// </summary>
    void Start()
    {
        string sdp_info = Client.SDPReceiver.Connect();
        if (sdp_info == "") {
            Debug.LogError("Could not retreive sdp file");
            SceneManager.LoadScene("ClientMain");
        }
        else {
            rtp_client_  = new Client.RTPClientWrapper(sdp_info);
            if (rtp_client_.Valid()) {
                int width = rtp_client_.GetWidth();
                int height = rtp_client_.GetHeight();
                display_texture_ = new Texture2D(width, height, TextureFormat.RGBA32, false);
            } else {
                Debug.LogError("Could not retreive rtp stream client");
                SceneManager.LoadScene("ClientMain");
            }
        }

        // Mirror X axis so that it's the CORRECT way
        m.mainTextureScale = new Vector2(-1, 1);
    }

    /// <summary>
    /// Attempt to retrieve the newest frame from the RTP stream and render it
    /// </summary>
    void LateUpdate()
    {
        byte[] frame = rtp_client_.GetNewFrame();
        if (frame != null) {
            display_texture_.LoadRawTextureData(frame);
            display_texture_.Apply();
            m.SetTexture("_MainTex", display_texture_);
        }
    }
}

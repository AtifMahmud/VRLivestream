using UnityEngine;
using UnityEngine.SceneManagement;
using UnityEngine.UI;
using Network;

namespace Client {

/// <summary>
/// Handles attempted connection to a server and a scene change to the VR scene.
/// This class is used in client main scene where the user enters the ip address
/// of the server.
/// </summary>
public class ConnectToServer : MonoBehaviour
{
    /// <summary>
    /// The name of the scene to connect to - should be entered in the Unity Editor
    /// </summary>
    public string scene_name_;

    /// <summary>
    /// The input field where the user is to input the IP address of the server
    /// </summary>
    public InputField input_field_;

    /// <summary>
    /// Attempts to connect to the server at the IP address parsed from the input field.
    /// On a successful connection to the server, the scene is switched to the scene
    /// with the name specified by the scene_name_ variable
    /// </summary>
    public void Connect() {
        string ip_address = GetIP();
        NetworkMaster.SetIP(ip_address);
        SceneManager.LoadScene(scene_name_);
    }

    /// <summary>
    /// Parses (TODO and validates) the ip address found in the input_field_
    /// </summary>
    /// <returns>The parsed IP address</returns>
    private string GetIP() {
        // TODO Error handle that the input is valid
        return input_field_.text;
    }
}

}  // namespace Client

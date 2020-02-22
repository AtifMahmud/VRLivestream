using UnityEngine;
using UnityEngine.SceneManagement;
using UnityEngine.UI;

namespace Client {

/// <summary>
/// Handles attempted connection to a server and a scene change to the VR scene.
/// This class is used in the client main scene where the user enters the ip address
/// of the server.
/// </summary>
public class ConnectToServer : MonoBehaviour
{
    // The port number of the server
    // This should match what is in the server code
    private const int SERVER_PORT = 10000;

    /// <summary>
    /// The name of the scene to connect to - should be entered in the Unity Editor
    /// </summary>
    public string scene_name_;

    /// <summary>
    /// The input field where the user is to input the IP address of the server
    /// </summary>
    public InputField input_field_;

    /// <summary>
    /// The text area, where instructions/errors are displayed
    /// </summary>
    public Text error_message_;

    /// <summary>
    /// Attempts to connect to the server at the IP address parsed from the input field.
    /// On a successful connection to the server, the scene is switched to the scene
    /// with the name specified by the scene_name_ variable
    /// </summary>
    public void Connect() {
        string ip_address = GetIP();

        if (ip_address != "0") {
            Client.SDPReceiver.SetIP(ip_address);
            Client.SDPReceiver.SetPort(GetPort());
            SceneManager.LoadScene(scene_name_);
        }
        else {
            error_message_.enabled = true;
        }
    }

    /// <summary>
    /// Parses and validates the ip address found in the input_field_
    /// </summary>
    /// <returns>The parsed IP address</returns>
    private string GetIP() {
        string ip = input_field_.text;
        string[] ipStringArr = ip.Split('.');
        int[] ipIntArr = new int[ipStringArr.Length];
        if (ipIntArr.Length != 4)
        {
            return "0";
        }
        else
        {
            for (int i = 0; i < ipStringArr.Length; i++)
            {
                int result = 0;
                if (int.TryParse(ipStringArr[i], out result)) {
                    ipIntArr[i] = int.Parse(ipStringArr[i]);
                    if (ipIntArr[i] < 0 || ipIntArr[i] > 255)
                    {
                        return "0";
                    }
                } else {
                    return "0";
                }

            }
        }
        return ip;
    }

    private int GetPort() {
        return SERVER_PORT;
    }
}

}  // namespace Client

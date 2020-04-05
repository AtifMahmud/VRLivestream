using System.Collections;
using UnityEngine;
using UnityEngine.XR;

namespace Client {

/// <summary>
/// This class will enable Google VR on a call to Start.
/// Can be attached to any object in a Scene where Cardboard VR needs to be enabled
/// It does not currently disable VR on destruction so a scene that is not in VR
/// will need to explicitly disable it.
///
/// This script should be attached to any object in the client scene for Google Cardboard
/// </summary>
public class StartCardboardVR : MonoBehaviour {
    /// <summary>
    /// Enables Google Cardboard VR on Start
    /// </summary>
    void Start ()
    {
        StartCoroutine(LoadDevice("Cardboard"));
    }

    /// <summary>
    /// Loads a VR device and enables VR
    /// </summary>
    /// <param name="device">The name of the VR Device to enable</param>
    private IEnumerator LoadDevice(string device)
    {
        XRSettings.LoadDeviceByName(device);
        yield return null;
        XRSettings.enabled = true;
    }
}

}  // namespace Client

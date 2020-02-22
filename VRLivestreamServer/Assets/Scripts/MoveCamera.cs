using System.Collections;
using System.Collections.Generic;
using UnityEngine;

/// <summary>
/// Convenience class for moving an object around the server scene
/// This class accepts keyboard inputs from the a, s, w, d keys
///
/// It should be attached to the main camera in the scene in order to move
/// the entire scene around
/// </summary>
public class MoveCamera : MonoBehaviour
{
    float flySpeed = .1f;

    /// <summary>
    /// Listen for keyboard input on every frame and move the attached object
    /// if input is detected
    /// </summary>
    void FixedUpdate()
    {
        if (Input.GetAxis("Vertical") != 0)
            transform.Translate(Vector3.forward * flySpeed * Input.GetAxis("Vertical"));

        if (Input.GetAxis("Horizontal") != 0)
            transform.Translate(Vector3.right * flySpeed * Input.GetAxis("Horizontal"));

        if (Input.GetKey(KeyCode.E))
            transform.Translate(Vector3.up * flySpeed);

        else if (Input.GetKey(KeyCode.Q))
            transform.Translate(Vector3.down * flySpeed);
    }
}

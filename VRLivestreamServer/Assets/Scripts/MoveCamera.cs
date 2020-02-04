using System.Collections;
using System.Collections.Generic;
using UnityEngine;

namespace Server {

public class MoveCamera : MonoBehaviour
{
    float flySpeed = .1f;
    GameObject defaultCam;
     
    bool shift;
    bool ctrl;
    float accelerationAmount = .00001F;
    float accelerationRatio = 2;
    float slowDownRatio = 0.2F;
 
    void Start()
    {
    }

    void FixedUpdate()
    {
    if (Input.GetAxis("Vertical") != 0)
    {
        transform.Translate(Vector3.forward * flySpeed * Input.GetAxis("Vertical"));
    }
   
   
    if (Input.GetAxis("Horizontal") != 0)
    {
        transform.Translate(Vector3.right * flySpeed * Input.GetAxis("Horizontal"));
    }
   
   
    if (Input.GetKey(KeyCode.E))
    {
        transform.Translate(Vector3.up * flySpeed);
    }
    else if (Input.GetKey(KeyCode.Q))
    {
        transform.Translate(Vector3.down * flySpeed);
    }
    } 
}

}  // namespace Server

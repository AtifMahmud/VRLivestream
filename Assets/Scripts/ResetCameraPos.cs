﻿using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class ResetCameraPos : MonoBehaviour
{
    // Start is called before the first frame update
    void Start()
    {
        
    }

    // Update is called once per frame
    void Update()
    {
        transform.localPosition = new Vector3(0, 0, 0);
    }

    private void LateUpdate()
    {
        transform.localPosition = new Vector3(0, 0, 0);
    }
}

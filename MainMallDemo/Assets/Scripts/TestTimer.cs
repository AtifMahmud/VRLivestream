using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using System;

public class TestTimer : MonoBehaviour
{
    private Text timeText;
    string time;

    // Start is called before the first frame update
    void Start()
    {
        timeText = gameObject.GetComponentInChildren<Text>();
    }

    // Update is called once per frame
    void Update()
    {
        time = System.DateTime.Now.ToString("yyyy/MM/dd\nhh:mm:ss");
        timeText.text = time;
    }
}

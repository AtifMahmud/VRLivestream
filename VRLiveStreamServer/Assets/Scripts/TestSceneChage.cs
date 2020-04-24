using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;
using System; 

public class TestSceneChage : MonoBehaviour
{

    long time;
      
    // Start is called before the first frame update
    void Start()
    {
        time = DateTime.Now.Millisecond;
    }

    // Update is called once per frame
    void Update()
    {
        if (DateTime.Now.Millisecond - time > 10000)
        {
            SceneManager.LoadScene("SceneChangeTest", LoadSceneMode.Additive);
        }
    }
}

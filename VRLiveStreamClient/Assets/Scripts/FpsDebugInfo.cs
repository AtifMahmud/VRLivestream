// toggle this on/off
//#define DEBUG
#if DEBUG

using UnityEngine;
using System.Collections.Generic;
using UnityEngine.UI;

/// <summary>
/// This script will display FPS in a little box in the
/// top left
/// </summary>
public class FpsDebugInfo : MonoBehaviour
{
    public string fps_text;
    int counter = 30; // calculate average FPS every 30 frames
    List<double> Intervals;
    List<double> Total;

    void Start()
    {
        Intervals = new List<double>();
        Total = new List<double>();
    }

    void OnGUI() {
        GUI.Label(new Rect (50,50,150,150), fps_text);
    }

    void LateUpdate()
    {
        Total.Add(Time.deltaTime);
        if(counter <=0 ){
            calcFPS();
            counter = 30;
        }
        Intervals.Add(Time.deltaTime);
        counter--;
    }

    public void calcFPS() {
        double sum = 0;
        double sum2 = 0;
        foreach(double i in Intervals) {
            sum += i;
        }
        foreach(double i in Total) {
            sum2 += i;
        }

        double average = sum/Intervals.Count;
        double fps = 1/average;

        double average2 = sum2/Total.Count;
        double fps2 =1/average2;
        int fps_int=(int)fps;
        int fps_int2=(int)fps2;
        fps_text = "Real-time FPS: " + fps_int + "\nAverage FPS:" + fps_int2;
        Intervals.Clear();
    }
}
#endif


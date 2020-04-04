// By default, the debug info is turned OFF, and public function RtpFpsUpdate
// will do nothing. uncomment the flag below to turn on the debugging.
#define FPS_DEBUG

using UnityEngine;
using UnityEngine.UI;
using System.Collections.Generic;
using System.Diagnostics;


namespace FpsDebug
{

/// <summary>
/// This script will display FPS in a little box in the
/// top left. It diplays application FPS and RTP stream FPS.
/// </summary>
public class FpsDebugInfo : MonoBehaviour
{
#if FPS_DEBUG
    // Variables for calculating application FPS
    public string app_fps_text;
    int app_fps_counter = 30; // calculate average FPS every 30 frames
    List<double> app_intervals = new List<double>();
    List<double> app_total = new List<double>();

    // Variables for calculating rtp stream FPS
    static string rtp_fps_text;
    static int rtp_counter = 30; // calculate average FPS every 30 frames
    static List<double> rtp_intervals = new List<double>();
    static List<double> rtp_total = new List<double>();
    static float rtp_timestamp = 0;
    static Stopwatch rtp_timer = new Stopwatch();

    void Start()
    {
        rtp_timer.Start();
    }

    void OnGUI() {
        GUI.Label(new Rect (100,100,250,250), app_fps_text + "\n" + rtp_fps_text);
    }

    void LateUpdate()
    {
        app_total.Add(Time.deltaTime);
        if(app_fps_counter <=0 ){
            calcAppFps();
            app_fps_counter = 30;
        }
        app_intervals.Add(Time.deltaTime);
        app_fps_counter--;
    }

    void calcAppFps() {
        double sum = 0;
        double sum2 = 0;
        foreach(double i in app_intervals) {
            sum += i;
        }
        foreach(double i in app_total) {
            sum2 += i;
        }

        double average = sum/app_intervals.Count;
        double fps = 1/average;

        double average2 = sum2/app_total.Count;
        double fps2 =1/average2;
        int fps_int=(int)fps;
        int fps_int2=(int)fps2;
        app_fps_text = "Real-time application FPS: " + fps_int + "\nAverage application FPS:" + fps_int2;
        app_intervals.Clear();
    }

    public static void RtpFpsUpdate()
    {
        rtp_intervals.Add((rtp_timer.ElapsedMilliseconds - rtp_timestamp)/1000.0f);
        rtp_total.Add((rtp_timer.ElapsedMilliseconds - rtp_timestamp)/1000.0f);
        rtp_timestamp = rtp_timer.ElapsedMilliseconds;
        if(rtp_counter <=0 ){
            calcRtpFPS();
            rtp_counter = 30;
        }
        rtp_counter--;
    }

    static void calcRtpFPS() {
        float sum = 0;
        float sum2 = 0;
        foreach(float i in rtp_intervals) {
            sum += i;
        }
        foreach(float i in rtp_total) {
            sum2 += i;
        }

        float average = sum/rtp_intervals.Count;
        float fps = 1/average;

        float average2 = sum2/rtp_total.Count;
        float fps2 =1/average2;
        int fps_int=(int)fps;
        int fps_int2=(int)fps2;
        rtp_fps_text = "Real-time RTP FPS: " + fps_int + "\nAverage RTP FPS: " + fps_int2;
        rtp_intervals.Clear();
    }
#else
    public static void RtpFpsUpdate()
    {
        // Do nothing
    }
#endif
}
}


using System;
using System.Text;
using System.Runtime.InteropServices;
using UnityEngine;

namespace Server {

/// <summary>
/// Wrapper class around the C++ RTP streaming code
/// </summary>
public class RTPServerWrapper {
    private IntPtr rtp_server_ = IntPtr.Zero;

    private const int SDP_OUT_SIZE = 1000;
    private const int ERR_SIZE = 100;
    private StringBuilder err_msg = new StringBuilder(ERR_SIZE);

    /// <summary>
    /// Constructs the underlying C++ RTP server object
    /// </summary>
    /// <param name="fps">The target fps of the stream</param>
    /// <param name="width">The width of each video frame, in pixels</param>
    /// <param name="height">The height of each video frame, in pixels</param>
    public RTPServerWrapper(int fps, int width, int height)
    {
        // Sometimes the creation of the server fails.
        // Try to create server up to a total of 3 times
        for (int i = 0; i < 3; ++i) {
            rtp_server_ = CreateServer(fps, width, height, err_msg, ERR_SIZE);
            if (rtp_server_ != IntPtr.Zero)  break;
        }

        if (rtp_server_ == IntPtr.Zero) {
            UnityEngine.Debug.LogError("Error: Could not create rtp stream.");
            UnityEngine.Debug.LogError("Error Message: " + err_msg.ToString());
        }
    }


    /// <summary>
    /// Destroys the underlying C++ RTP server object
    /// </summary>
    ~RTPServerWrapper()
    {
        DestroyServer(rtp_server_);
    }

    /// <summary>
    /// Sends one image frame to the server, which will encode the frame and then
    /// send it over RTP to all connected clients
    /// </summary>
    /// <param name="data">The raw image data of the frame</param>
    public void SendFrameToServer(byte[] data)
    {
        int err = SendRawFrame(rtp_server_, data, data.Length, err_msg, ERR_SIZE);

        if (err < 0)
            Debug.LogError(
                "Error Sending Frame to Server: " +
                err.ToString() + ". " +
                err_msg.ToString());
    }

    /// <summary>
    /// Add a new client to the RTP server
    /// </summary>
    /// <param name="client_ip">The ip address of the client machine</param>
    /// <returns>
    /// The SDP file as a string.false The sdp file hold the necessary
    /// info for the client to connect to the stream
    /// </returns>
    public string AddClientToServer(string client_ip)
    {
        var str_builder = new StringBuilder(client_ip);
        var sdp_out = new StringBuilder(SDP_OUT_SIZE);
        int err = AddClient(rtp_server_, str_builder, sdp_out, SDP_OUT_SIZE, err_msg, ERR_SIZE);

        if (err < 0) {
            Debug.LogError("Unable to add client: " + err.ToString());
            Debug.LogError(err_msg.ToString());
            return "";
        }

        Debug.Log("Sending sdp content of: " + sdp_out.ToString());
        return sdp_out.ToString();
    }

    /***************************************************************************
    * C++ API functions
    ***************************************************************************/

    [DllImport("librtpserver", CharSet = CharSet.Ansi)]
    private static extern IntPtr CreateServer(
        int fps,
        int width,
        int height,
        StringBuilder err_msg,
        int err_msg_size);

    [DllImport("librtpserver", CharSet = CharSet.Ansi)]
    private static extern void DestroyServer(IntPtr server);

    [DllImport("librtpserver", CharSet = CharSet.Ansi)]
    private static extern int SendRawFrame(
        IntPtr server,
        byte[] raw_data,
        int raw_length,
        StringBuilder err_msg,
        int err_msg_size);

    [DllImport("librtpserver", CharSet = CharSet.Ansi)]
    private static extern int AddClient(
        IntPtr server,
        StringBuilder client_ip,
        StringBuilder sdp_output,
        int sdp_buff_size,
        StringBuilder err_msg,
        int err_msg_size);
}

}  // namespace Server

using UnityEngine;
using System.Net.Sockets;

namespace Client {

/// <summary>
/// Creates a TCP client socker in order to retreive the SDP file from the server
/// that is necessary to connect to the RTP stream
/// </summary>
public static class SDPReceiver {
    private const int SDP_BUF_SIZE = 1000;
    private static string ip_ = "";
    private static int port_ = 0;

    /// <summary>
    /// Set the ip of the server to connect to
    /// </summary>
    public static void SetIP(string ip)
    {
        ip_ = ip;
    }

    /// <summary>
    /// Sets the port of the TCP server to connect to
    /// </summary>
    public static void SetPort(int port)
    {
      port_ = port;
    }

    /// <summary>
    /// Attempts to connect to the Server
    ///
    /// SetIP() and SetPort() should be called before this function
    /// </summary>
    /// <returns></returns>
    public static string Connect()
    {
        if (port_ == 0 || ip_ == "") {
            Debug.LogError("Cannot connect to server before setting ip address and port");
            return "";
        }

        try {
            TcpClient client = new TcpClient(ip_, port_);

            // Send a "Register" request to the server
            byte[] data = System.Text.Encoding.ASCII.GetBytes("Register");
            NetworkStream stream = client.GetStream();
            stream.Write(data, 0, data.Length);

            // Get the sdp content back as the response.
            data = new byte[SDP_BUF_SIZE];
            int size = stream.Read(data, 0, data.Length);
            string sdp_content = System.Text.Encoding.ASCII.GetString(data, 0, size);

            System.String str = System.Text.Encoding.ASCII.GetString(data, 0, size);
            Debug.Log("SDP Content\n" + str);
            Debug.Log("Length: " + str.Length);

            // Close everything.
            stream.Close();
            client.Close();

            return sdp_content;
        } catch (System.ArgumentNullException e) {
            Debug.Log("Caught socket exception: " + e.ToString());
        } catch (System.Net.Sockets.SocketException e) {
            Debug.Log("Caught socket exception: " + e.ToString());
        }

        return "";
    }
}

}  // namespace Client

using System;
using System.IO;
using System.Net;
using System.Net.Sockets;
using System.Text;
using UnityEngine;

namespace Server {

/// <summary>
/// A TCP server that handles connecting clients to the RTP stream by supplying
/// them with SDP file info. Only one of instance of this class should be constructed
/// </summary>
class SdpSender {
    private const int SERVER_PORT = 10000;
    private const int SERVER_BUF_SIZE = 100;

    private TcpListener server_;
    private RTPStreamAsyncHandler stream_task_;

    /// <summary>
    /// Constructs a new SDPSender object
    /// </summary>
    /// <param name="stream_task">
    /// The streaming task that can be used to add clients to the RTP server
    /// </param>
    public SdpSender(RTPStreamAsyncHandler stream_task)
    {
        Debug.Log("Start listener");
        server_ = new TcpListener(IPAddress.Any, SERVER_PORT);
        server_.Start();
        stream_task_ = stream_task;
    }

    /// <summary>
    /// Attempts to accept one TCP request from a client.
    /// Upon receiving a request, it will connect it with the RTP server
    /// </summary>
    public void AcceptRequests()
    {
        try {
            Byte[] bytes = new Byte[SERVER_BUF_SIZE];
            TcpClient client = server_.AcceptTcpClient();
            NetworkStream stream = client.GetStream();

            int size = stream.Read(bytes, 0, bytes.Length);
            System.String data = System.Text.Encoding.ASCII.GetString(bytes, 0, size);

            if (data == "Register") {
              Debug.Log("Register client");
            } else {
              // Log a warning but ultimately igore the error and attempt
              // to send data to the client
              Debug.LogWarning("Unknown request from client: " + data);
            }

            string ip_address = ((IPEndPoint)client.Client.RemoteEndPoint).Address.ToString();
            string sdp_content = stream_task_.AddClient(ip_address);

            byte[] msg = System.Text.Encoding.ASCII.GetBytes(sdp_content);
            stream.Write(msg, 0, msg.Length);
            client.Close();
        }
        catch(SocketException e)
        {
            Debug.Log("Could not send sdp file: " + e.ToString());
        }
   }
    /// <summary>
    /// Closes the unerlying TCP server
    /// </summary>
    ~SdpSender()
    {
        server_.Stop();
    }
}

}  // namespace Server

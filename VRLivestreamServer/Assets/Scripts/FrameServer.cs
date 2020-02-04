using NetMQ;
using NetMQ.Sockets;
using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.Assertions;

namespace FS {
public class FrameServer {
    private int _port = 10000;
    private PublisherSocket _server;

    public FrameServer() {
        AsyncIO.ForceDotNet.Force();
        NetMQConfig.Linger = new TimeSpan(0,0,1);

        _server = new PublisherSocket();
        _server.Options.Linger = new TimeSpan(0,0,1);
        _server.Bind($"tcp://*:{_port}");
        Debug.Log($"server on {_port}");

        Assert.IsNotNull(_server);
    }

    public void Cleanup() {
        _server?.Dispose();
        NetMQConfig.Cleanup(false);
    }

    public void SendFrame(byte[] frame) {
        _server
            .SendMoreFrame("frames") // Topic 
            .SendFrame(frame);
    }
}
}

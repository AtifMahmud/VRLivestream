using NetMQ;
using NetMQ.Sockets;
using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.Assertions;

namespace FS {
public class FrameClient {
    private int _port = 10000;
    private SubscriberSocket _client;

    public FrameClient() {
        AsyncIO.ForceDotNet.Force();
        NetMQConfig.Linger = new TimeSpan(0,0,1);

        var ip = Network.NetworkMaster.GetIP();
        _client = new SubscriberSocket();
        _client.Options.Linger = new TimeSpan(0,0,1);
        _client.Connect($"tcp://{ip}:{_port}");
        Debug.Log($"client on {_port}");

        Assert.IsNotNull(_client);
        _client.Subscribe("frames");
    }

    public void Cleanup() {
        _client?.Dispose();
        NetMQConfig.Cleanup(false);
    }

    public byte[] TryReceiveFrame() {
        String unused;
        if (_client.TryReceiveFrameString(out unused)) {
            return _client.ReceiveFrameBytes();
        } else {
            return null;
        }
    }
}
}

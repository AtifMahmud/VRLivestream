using NetMQ;
using NetMQ.Sockets;

namespace UniCast {

public static class Client {  
    private static SubscriberSocket subscriber;
    
    static Client() 
    {
      subscriber = new SubscriberSocket();
      subscriber.Connect("tcp://127.0.0.1:5556");
    }  

    public static byte[] ReceiveFrame() {
      return subscriber.ReceiveFrameBytes();
    }
}

}  // namespace UniCast

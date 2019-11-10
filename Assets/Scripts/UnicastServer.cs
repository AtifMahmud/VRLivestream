using NetMQ;
using NetMQ.Sockets;

namespace UniCast {

public static class Server {  
    private static PublisherSocket publisher;
    
    static Server() 
    {
      publisher = new PublisherSocket();
      publisher.Bind("tcp://*:5556");
    }  

    public static void SendFrame(byte[] frame) {
      publisher.SendFrame(frame);
    }
}

}  // namespace UniCast

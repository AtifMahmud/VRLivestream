using System;
using System.Text;
using System.Threading.Tasks;

namespace Server {

/// <summary>
/// This class is used to move the RTP functionality out of the main thread
/// with the intent of stabalizing the framerate.
/// </summary>
public class RTPStreamAsyncHandler {
    private RTPServerWrapper rtp_server_;
    private Task task_;

    /// <summary>
    /// Construct a new RTPStreamAsyncHandler object
    /// This will create the underling RTPServerWrapper object
    /// </summary>
    /// <param name="framerate">The desired framerate of the stream</param>
    /// <param name="width">The width of each image frame, in pixels</param>
    /// <param name="height">The height of each image frame, in pixels</param>
    /// <param name="codec_speed">slower speeds give better compression. more info at https://trac.ffmpeg.org/wiki/Encode/H.264</param>
    /// <param name="codec_type">0 for H264, 1 for H265</param>
    public RTPStreamAsyncHandler(int framerate, int width, int height, string codec_speed, int codec_type)
    {
        rtp_server_  = new RTPServerWrapper(framerate, width, height, codec_speed, codec_type);
    }

    /// <summary>
    /// Will send a frame asyncrhonously to the RTP stream.
    /// If the last frame has not yet finished sending, this call will be blocked
    /// until it is completed. This ensures that there is only one RTP task active
    /// at one time, as it is not thread-safe to send multiple frames to the server
    /// on different threads concurrently.
    /// </summary>
    /// <param name="img_data">The raw data for the image frame to send</param>
    public void SendFrameAsync(byte[] img_data)
    {
        if (task_ != null)
            task_.Wait();

        task_ = Task.Run(() => {
            rtp_server_.SendFrameToServer(img_data);
        });
    }

    /// <summary>
    /// Adds a client to the underlying RTP Server wrapper
    /// </summary>
    /// <param name="client_ip">The ip address of the client</param>
    /// <returns>The sdp file info needed to connect the client to the RTP stream</returns>
    public string AddClient(string client_ip)
    {
        return rtp_server_.AddClientToServer(client_ip);
    }
}

}  // namespace Server

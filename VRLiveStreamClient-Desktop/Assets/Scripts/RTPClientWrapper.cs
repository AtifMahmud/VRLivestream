using System;
using System.Text;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using UnityEngine;

namespace Client {

/// <summary>
/// Class that wraps the RTP Client C++ code
/// </summary>
public unsafe class RTPClientWrapper {
    private IntPtr rtp_client_ = IntPtr.Zero;
    private List<byte[]> decoded_data_;

    private const int ERR_SIZE = 100;
    private StringBuilder err_msg = new StringBuilder(ERR_SIZE);

    private const int MAX_FRAMES = 30;

    private int frame_width_  = 0;
    private int frame_height_ = 0;

    /// <summary>
    /// Constructs the new Wrapper object
    /// Valid() should be called afterwards to check if the construction was successful
    /// </summary>
    /// <param name="sdp_file_content"></param>
    public RTPClientWrapper(string sdp_file_content)
    {
        // Directory where the files can be written to
        char[] data_pth = (UnityEngine.Application.persistentDataPath).ToCharArray();
        char dir_seperator = System.IO.Path.DirectorySeparatorChar;

        // Switch to '\' seperators if on windows
        data_pth = FixIfWindowsPath(data_pth, dir_seperator);

        rtp_client_ = CreateRTPClient(
            sdp_file_content, err_msg, ERR_SIZE, data_pth, dir_seperator);

        if (rtp_client_ == IntPtr.Zero) {
            Debug.LogError("Could not create rtp client");
            Debug.LogError(err_msg.ToString());
            return;
        }

        // The buffer size is the size of the array needed to hold one decoded image frame
        int buffer_size = GetBufferSize(rtp_client_, err_msg, ERR_SIZE);
        if (buffer_size <= 0) {
            Debug.LogError("Error, could not get buffer size!");
            Debug.LogError(err_msg.ToString());
            DestroyRTPClient(rtp_client_);
            rtp_client_ = IntPtr.Zero;
            return;
        }

        // Creates a set of buffers and adds them to the client
        // The more buffers we have, the farther behind the client can get,
        // but the less likely frames are dropped if the C# side gets temporarily
        // behind the C++ side
        decoded_data_ = new List<byte[]>();
        for (int i = 0; i < MAX_FRAMES; ++ i) {
            var arr = new byte[buffer_size];
            decoded_data_.Add(arr);
            int success = AddBuffer(rtp_client_, arr, err_msg, ERR_SIZE);

            if (success < 0) {
                Debug.LogError("Error, could not add buffer");
                Debug.LogError(err_msg.ToString());
                DestroyRTPClient(rtp_client_);
                rtp_client_ = IntPtr.Zero;
                return;
            }
        }

        // Grab the widgh and height of the stream
        if (!SetWidthAndHeight()) {
            DestroyRTPClient(rtp_client_);
            rtp_client_ = IntPtr.Zero;
            return;
        }

        // Attempt to start the stream - must be done after buffers are added
        int err = Start(rtp_client_, err_msg, ERR_SIZE);
        if (err != 0) {
            Debug.LogError("Error: Could not start rtp client: " + err);
            Debug.LogError(err_msg.ToString());
            DestroyRTPClient(rtp_client_);
            rtp_client_ = IntPtr.Zero;
        }
    }

    /// <summary>
    /// Destroys the underlying C++ stream object
    /// </summary>
    ~RTPClientWrapper()
    {
        DestroyRTPClient(rtp_client_);
    }

    /// <summary>
    /// Returns true if the rtp client was successfully constructed
    /// </summary>
    public bool Valid()
    {
        return rtp_client_ != IntPtr.Zero;
    }

    /// <summary>
    /// Get the newest image frame from the stream
    /// </summary>
    public byte[] GetNewFrame()
    {
        int index = GetCurrBuffIndex(rtp_client_, err_msg, ERR_SIZE);
        if (index >= 0 && index < decoded_data_.Count)
            return decoded_data_[index];
        else return null;
    }

    /// <summary>
    /// Sets the image and height variables by reading the width and height
    /// from the stream. If the width and height could not be read, this function
    /// will return false. Otherwise it will return true.
    /// </summary>
    private bool SetWidthAndHeight()
    {
      int success = 0;
        fixed (int* width_ptr = &frame_width_) {
            fixed (int* height_ptr = &frame_height_) {
                success = GetFrameDims(
                    rtp_client_, width_ptr, height_ptr, err_msg, ERR_SIZE);
            }
        }
        if (success != 0) {
            Debug.LogError("Could not read stream height and width");
            Debug.LogError(err_msg.ToString());
            return false;
        }
        return true;
    }

    /// <summary>
    /// Get the height of the stream
    /// </summary>
    public int GetHeight() { return frame_height_; }

    /// <summary>
    /// Get the width of the stream
    /// </summary>
    public int GetWidth() { return frame_width_; }

    /// <summary>
    /// Fixes the data path if the system running the program is a windows OS
    /// </summary>
    /// <param name="data_pth">The path</param>
    /// <param name="dir_seperator">The directory seperator for the host OS</param>
    /// <returns>The adjusted data path. Will be unaltered if the host OS is not Windows</returns>
    private char[] FixIfWindowsPath(char[] data_pth, char dir_seperator)
    {
        // Unity seems to return a path with '/' seperators, which doesn't work
        // when passing to C++ on windows
        if (dir_seperator != '/') {
            int count = 0;
            for (int i = 0; i < data_pth.Length; ++i) {
                if (data_pth[i] == '/')
                    count++;
            }

            char[] altered_pth = new char[data_pth.Length + count];
            int offset = 0;
            for (int i = 0; i < data_pth.Length; ++i) {
                if (data_pth[i] == '/') {
                    altered_pth[i + offset] = dir_seperator;
                    altered_pth[i + 1 + offset] = dir_seperator;
                    ++offset;
                } else {
                altered_pth[i + offset] = data_pth[i];
                }
            }
            return altered_pth;
        }

        // If the dir_seperator is a '/' do nothing
        return data_pth;
    }

    /***************************************************************************
    * C++ API Functions
    ***************************************************************************/

    [DllImport("librtpclient", CharSet = CharSet.Ansi)]
    private static extern IntPtr CreateRTPClient(
        string sdp_content,
        StringBuilder err_msg,
        int buf_size,
        char[] path,
        char dir_delim);

    [DllImport("librtpclient", CharSet = CharSet.Ansi)]
    private static extern void DestroyRTPClient(IntPtr client);

    [DllImport("librtpclient", CharSet = CharSet.Ansi)]
    private static extern int GetCurrBuffIndex(
        IntPtr client,
        StringBuilder err_msg,
        int buf_size);

    [DllImport("librtpclient", CharSet = CharSet.Ansi)]
    private static extern int GetBufferSize(
        IntPtr client,
        StringBuilder err_msg,
        int buf_size);

    [DllImport("librtpclient", CharSet = CharSet.Ansi)]
    private static extern int AddBuffer(
        IntPtr client,
        byte[] buffer,
        StringBuilder err_msg,
        int buf_size);

    [DllImport("librtpclient", CharSet = CharSet.Ansi)]
    private static extern int GetFrameDims(
        IntPtr client,
        int* width,
        int* height,
        StringBuilder err_msg,
        int buf_size);

    [DllImport("librtpclient", CharSet = CharSet.Ansi)]
    private static extern int Start(
        IntPtr client,
        StringBuilder err_msg,
        int buf_size);
}

}  // namespace Client

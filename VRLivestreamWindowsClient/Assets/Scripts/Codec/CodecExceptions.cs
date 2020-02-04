using System;

namespace Codec {

/// <summary>
/// The base exception type for all exceptions in the Codec namespace
/// </summary>
public class CodecException : Exception {
    public CodecException(string err_message) : base(err_message)
    {}
}

/// <summary>
/// Exception to be thrown when exception is caused by an ffmpeg function
/// returning an error code
/// </summary>
public class FFMpegException : CodecException {
    private readonly int err_code_;
    public FFMpegException(string err_message, int err_code) : base(err_message)
    {
        err_code_= err_code;
    }
    public int ErrCode() { return err_code_; }
}

}  // namespace Codec

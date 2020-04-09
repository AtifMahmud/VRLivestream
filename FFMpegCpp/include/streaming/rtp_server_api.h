/**
 * @file rtp_server_api.h
 * @brief Holds the API that can be used from Unity C# code
 *        for the server-side application. Only functions defined in this
 *        file can be used by the server-side Unity C# application
 *
 * Note that all error-codes returned by these functions match FFMpeg error codes,
 * except in the case of -1, which indicates that the error did not originate from
 * FFMpeg, or there was no FFMpeg error-code to return.
 */

#pragma once

#include "streaming/rtp_server.h"

#ifdef _WIN32
#define EXPORT __declspec(dllexport)
#else
#define EXPORT __attribute__((visibility("default")))
#endif

extern "C" {

/**
 * @brief Create the RTPServer object
 *
 * @param fps The desired fps of the stream
 * @param width The width of the image frames to encode
 * @param height The height of the image frames to encode
 * @param codec_speed for example, "ultrafast", "medium". (https://trac.ffmpeg.org/wiki/Encode/H.264)
 * @param codec_type select between h264 and h265. values can be found in frame_encoder.h
 * @param err_msg An output character array to hold an error message in the case of failure
 * @param err_msg_size The size of the output character array
 *
 * @return A pointer to the created RTPServer object, or a null pointer in the case of failure
 *         The C# should hold the returned pointer in an IntPtr object and delete
 *         if by calling DestroyServer when done with it
 */
EXPORT codec::stream::RTPServer* CreateServer(
    int fps,
    int width,
    int height,
    char* codec_speed,
    int codec_type,
    char* err_msg,
    int err_msg_size);

/**
 * @brief Destroy the server constructed with the CreateServer call.
 *        The passed in pointer will be invalid after calling this function
 */
EXPORT void DestroyServer(codec::stream::RTPServer* server);

/**
 * @brief Send a raw frame to all connected clients.
 *
 * Under the hood, this function will first encode the frame, before sending it
 * Note that it is thread-safe to call this function and AddClient in parallel
 *
 * @param server The pointer to the RTPServer object
 * @param raw_data The raw data representing the image frame
 * @param raw_length The length of the raw data
 * @param err_msg An output character buffer to put an error message in, in the case of failure
 * @param err_msg_size The length of the output character buffer
 *
 * @return 0, or a negative error code
 */
EXPORT int SendRawFrame(
    codec::stream::RTPServer* server,
    uint8_t* raw_data,
    int raw_length,
    char* err_msg,
    int err_msg_size);

/**
 * @brief Add a client to the server. All frames sent by calling SendRawFrame
 *        will be sent to this client after calling this function
 *
 * Note that it is thread-safe to call this function and SendRawFrame in parallel
 *
 * @param server Pointer to the RTPServer object
 * @param client_ip The ip address of the client to add
 * @param sdp_output Output character buffer to put sdp file content in
 *                   This sdp file content should be sent to the client
 *                   in order for the client to connect to the strean
 * @param sdp_buff_size The size of the sdp buffer. Should be large enough
 *                      to hold an average-sized sdp file. ~1000 characters
 *                      should be more than sufficient
 * @param err_msg An output character buffer to put an error message in, in the case of failure
 * @param err_msg_size The size of the error message buffer
 *
 * @return 0, or a negative error-code
 */
EXPORT int AddClient(
    codec::stream::RTPServer* server,
    char* client_ip,
    char* sdp_output,
    int sdp_buff_size,
    char* err_msg,
    int err_msg_size);

}  // extern

/**
 * @file rtp_client_api.h
 * @brief Exposed API for using in Unity
 *
 * This wraps the RTPClient in a way that is usable in C# Unity code.
 * Only functions exposed here are available to use in C#
 *
 * All error codes returned by the API match FFMpeg error codes, except in the
 * case of -1, which is indicates the error did not originate from the FFMpeg library.
 * The only exception to this is in the case of the GetCurrBuffIndex function
 */

#pragma once

#include "streaming/rtp_client.h"

#ifdef _WIN32
#define EXPORT __declspec(dllexport)
#else
#define EXPORT __attribute__((visibility("default")))
#endif

extern "C" {

/**
 * @brief Create a new RTP client. The C# can hold the constructed pointer
 *        in an IntPtr object. After it has been used, the DestroyRTPClient
 *        function should be called to delete it
 *
 * @param sdp_content The sdp content from the RTPServer to connect to the stream with
 * @param err_msg An out char array that will hold an error message in the case
 *                of a failure in construction
 * @param err_buf_size The size of the output err message array
 * @param writable_dir A directory where the app has write permissions
 * @param dir_delim The directory deliminator for the host OS
 *
 * @return A pointer to the constructed client, or a nullptr if it was unable
 *         to be constructed.
 */
EXPORT codec::stream::RTPClient* CreateRTPClient(
  const char* sdp_content,
  char* err_msg,
  int err_buf_size,
  const char* writable_dir,
  char dir_delim);

/**
 * @brief Destroy a client constructed using CreateRTPClient
 *        The supplied pointer will no longer be valid after calling this function
 */
EXPORT void DestroyRTPClient(codec::stream::RTPClient* client);

/**
 * @brief Get the required size of a buffer needed to hold a decoded image frame
 *
 * @param client A pointer to the RTPClient
 * @param err_msg An output character buffer to put an error message in, in the case of failure
 * @param err_buf_size The size of the output character buffer
 *
 * @return The size of the required buffer, or a negative error code
 */
EXPORT int GetBufferSize(
    codec::stream::RTPClient* client, char* err_msg, int err_buf_size);

/**
 * @brief Add a buffer to the Client. At least 3 buffers need to be added before
 *        the RTPClient can be started
 *
 * @param client The RTPClient pointer
 * @param buffer The buffer to add
 * @param err_msg An output character buffer to put an error message in, in the case of failure
 * @param err_buf_size The size of the output character buffer
 *
 * @return 0, or a negative error code
 */
EXPORT int AddBuffer(
    codec::stream::RTPClient *client,
    uint8_t* buffer,
    char* err_msg,
    int err_buf_size);

/**
 * @brief Start the RTPClient
 *
 * This function should be called after adding at least 3 buffers to hold
 * decoded frame data.
 *
 * @param client Pointer to the RTPClient to start
 * @param err_msg An output char array to hold an error message in the case of an error
 * @param err_buf_size The size of the output error message array
 *
 * @return 0, or a negative error code on failure
 */
EXPORT int Start(
    codec::stream::RTPClient* client, char* err_msg, int err_buf_size);

/**
 * @brief Get the index of the added buffer that holds the most recent decoded
 *        data frame. This is 0 indexed, so the first buffer added will correspond
 *        to a return value of 0, the second a return value of 1, and so on
 *
 * @param client The RTPClient pointer
 * @param err_msg An output char buffer to put an error message in, in case of an error
 * @param err_buf_size The size of the output buffer
 *
 * @return The index of the most recent buffer, or a negative error code
 *         A value of -1 in this case indicates no new frame has been retreived
 *         since the last call. -2 indicates a generic exception
 */
EXPORT int GetCurrBuffIndex(
    codec::stream::RTPClient* client, char* err_msg, int err_buf_size);

/**
 * @brief Get the dimensions of decoded image frames
 *
 * @param client The RTPClient pointer
 * @param width Output pointer to put the width in
 * @param height Output pointer to put the height in
 * @param err_msg Output character buffer to put an error message in, in the case of failure
 * @param err_buf_size The size of the output error message buffer
 *
 * @return 0, or a negative error code on failure
 */
EXPORT int GetFrameDims(
    codec::stream::RTPClient* client,
    int* width,
    int* height,
    char* err_msg,
    int err_buf_size);

}  // extern

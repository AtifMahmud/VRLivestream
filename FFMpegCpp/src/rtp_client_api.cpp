/**
 * @file rtp_client_api.cpp
 * @brief Implementation of the RTPClient api
 */

#include "streaming/rtp_client_api.h"

#include <cstring>
#include <functional>

#include "common/codec_exception.h"

static const char* unexpected_exception =
    "RTP Client API caught unexpected exception";

static const char* null_err = "Supplied client is null";

namespace {

// Windows gives warning for using strncpy and suggests to use srncpy_s
// which is only available on Windows.
// For other platforms, just use strncpy
#ifdef WIN32
void CopyCString(char *const dest, const char* src, const int size)
{
  if (dest != NULL && src != NULL)
    strncpy_s(dest, size, src, size);
}

#else
void CopyCString(char *const dest, const char* src, const int size)
{
  if (dest != NULL && src != NULL)
    strncpy(dest, src, size);
}

#endif

inline int ReturnNullErr(char *const err_msg, const int err_buf_size)
{
  CopyCString(err_msg, null_err, err_buf_size);
  return -1;
}

}  // namespace

// Unity Interface
extern "C" {

codec::stream::RTPClient* CreateRTPClient(
    const char *const sdp_content,
    char *const err_msg,
    const int err_buf_size,
    const char *const path,
    const char dir_delim)
{
  try {
    auto client = new codec::stream::RTPClient(sdp_content, path, dir_delim);
    return client;
  } catch (const std::exception &e) {
    CopyCString(err_msg, e.what(), err_buf_size);
    return NULL;
  } catch (...) {
    CopyCString(err_msg, unexpected_exception, err_buf_size);
    return NULL;
  }
}

void DestroyRTPClient(codec::stream::RTPClient *const client)
{
  if (client) delete client;
}

int Start(
    codec::stream::RTPClient *const client,
    char *const err_msg,
    const int err_buf_size)
{
  if (!client)
    return ReturnNullErr(err_msg, err_buf_size);

  try {
    client->Start();
    return 0;
  } catch (const codec::CodecException &e) {
    CopyCString(err_msg, e.what(), err_buf_size);
    return e.ErrorCode();
  } catch (const std::exception &e) {
    CopyCString(err_msg, e.what(), err_buf_size);
    return -1;
  } catch (...) {
    CopyCString(err_msg, unexpected_exception, err_buf_size);
    return -1;
  }
}

int GetCurrBuffIndex(
    codec::stream::RTPClient *const client,
    char *const err_msg,
    const int err_buf_size)
{
  if (!client)
    return ReturnNullErr(err_msg, err_buf_size);

  try {
    return client->GetCurrBuffIndex();
  } catch (const codec::CodecException &e) {
    CopyCString(err_msg, e.what(), err_buf_size);
    return e.ErrorCode();
  } catch (const std::exception &e) {
    CopyCString(err_msg, e.what(), err_buf_size);
    return -1;
  } catch (...) {
    CopyCString(err_msg, unexpected_exception, err_buf_size);
    return -1;
  }
}

int GetBufferSize(
    codec::stream::RTPClient *const client,
    char *const err_msg,
    const int err_buf_size)
{
  if (!client)
    return ReturnNullErr(err_msg, err_buf_size);

  try {
    return client->GetBufferSize();
  } catch (const codec::CodecException &e) {
    CopyCString(err_msg, e.what(), err_buf_size);
    return e.ErrorCode();
  } catch (const std::exception &e) {
    CopyCString(err_msg, e.what(), err_buf_size);
    return -1;
  } catch (...) {
    CopyCString(err_msg, unexpected_exception, err_buf_size);
    return -1;
  }
}

int AddBuffer(
    codec::stream::RTPClient *const client,
    uint8_t *const buffer,
    char *const err_msg,
    const int err_buf_size)
{
  if (!client)
    return ReturnNullErr(err_msg, err_buf_size);

  try {
    client->AddBuffer(buffer);
    return 0;
  } catch (const codec::CodecException &e) {
    CopyCString(err_msg, e.what(), err_buf_size);
    return e.ErrorCode();
  } catch (const std::exception &e) {
    CopyCString(err_msg, e.what(), err_buf_size);
    return -1;;
  } catch (...) {
    CopyCString(err_msg, unexpected_exception, err_buf_size);
    return -1;
  }
}

int GetFrameDims(
    codec::stream::RTPClient *const client,
    int *const width,
    int *const height,
    char *const err_msg,
    const int err_buf_size)
{
  if (!client)
    return ReturnNullErr(err_msg, err_buf_size);

  try {
    client->GetFrameDimensions(width, height);
    return 0;
  } catch (const codec::CodecException &e) {
    CopyCString(err_msg, e.what(), err_buf_size);
    return e.ErrorCode();
  } catch (const std::exception &e) {
    CopyCString(err_msg, e.what(), err_buf_size);
    return -1;;
  } catch (...) {
    CopyCString(err_msg, unexpected_exception, err_buf_size);
    return -1;
  }
}

}  // extern

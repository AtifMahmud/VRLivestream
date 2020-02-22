/**
 * @file rtp_server_api.cpp
 * @brief Implementation of the RTPServer
 */

#include <cstring>

#include "common/codec_exception.h"
#include "streaming/rtp_server_api.h"

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

int ReturnErr(
    char *const err_msg_buf,
    const int err_msg_buf_size,
    const char *const err_msg,
    const int err_code)
{
  CopyCString(err_msg_buf, err_msg, err_msg_buf_size);
  return err_code;
}

}  // namespace

// Unity Interface
extern "C" {

codec::stream::RTPServer* CreateServer(
    const int fps,
    const int width,
    const int height,
    char *const err_msg,
    const int err_msg_size)
{
  try {
    auto server = new codec::stream::RTPServer(fps, width, height);
    return server;
  } catch (const codec::CodecException &e) {
    CopyCString(err_msg, e.what(), err_msg_size);
    return NULL;
  } catch (const std::exception &e) {
    CopyCString(err_msg, e.what(), err_msg_size);
    return NULL;
  } catch (...) {
    CopyCString(err_msg, "Unexpected Excpetion", err_msg_size);
    return NULL;
  }
}

void DestroyServer(codec::stream::RTPServer *const server)
{
  if (server) delete server;
}

int SendRawFrame(
    codec::stream::RTPServer *const  server,
    uint8_t *const raw_data,
    const int raw_length,
    char *const err_msg,
    const int err_msg_size)
{
  if (!server)
      return -1;

  try {
    server->SendRawFrame(raw_data, raw_length);
    return 0;
  } catch (const codec::CodecException &e) {
    CopyCString(err_msg, e.what(), err_msg_size);
    return e.ErrorCode();
  } catch (const std::exception &e) {
    CopyCString(err_msg, e.what(), err_msg_size);
    return -1;
  }
  catch (...) {
    CopyCString(err_msg, "Unexpected Exception", err_msg_size);
    return -1;
  }
}

int AddClient(
    codec::stream::RTPServer *const server,
    char *const client_ip,
    char *const sdp_output,
    const int sdp_buff_size,
    char *const err_msg,
    const int err_msg_size)
{
  if (!server || !client_ip || !sdp_output)
    return -1;

  try {
    std::string ip_str(client_ip);
    std::string sdp_content = server->AddStream(client_ip);

    // + 1 to include null-terminator
    if (sdp_content.size() + 1 > sdp_buff_size) {
      CopyCString(
          err_msg,
          "Supplied sdp buffer not large enough to fit sdp file",
          err_msg_size);
      return -1;
    }

    CopyCString(sdp_output, sdp_content.c_str(), sdp_buff_size);
    return 0;
  } catch (const codec::CodecException &e) {
    CopyCString(err_msg, e.what(), err_msg_size);
    return e.ErrorCode();
  } catch (const std::exception &e) {
    CopyCString(err_msg, e.what(), err_msg_size);
    return -1;
  } catch (...) {
    CopyCString(err_msg, "Unexpected Exception", err_msg_size);
    return -1;
  }
}

}  // extern

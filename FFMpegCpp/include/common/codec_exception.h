/**
 * @file codec_exception.h
 * @brief Contains an exception type for handling FFMpeg errors
 */

#pragma once

#include <string>
#include <stdexcept>

namespace codec {

/**
 * @brief Exception to throw run FFMpeg returns an error code.
 */
struct CodecException : public std::runtime_error {
  /**
   * @brief Construct a new CodecException object
   *
   * @param code The error code from FFMpeg
   * @param msg An error message
   */
  CodecException(int code, const std::string &msg)
      : std::runtime_error(msg), err_code(code)
  {}

  /**
   * @brief Get the exception message
   */
  const char* what() const noexcept override {
    return std::runtime_error::what();
  }

  /**
   * @brief Get the error code
   */
  int ErrorCode() const noexcept {
    return err_code;
  }

 private:
  int err_code;
};

}  // namespace codec

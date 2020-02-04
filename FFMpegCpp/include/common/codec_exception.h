#pragma once

#include <string>
#include <stdexcept>

namespace codec {

struct CodecException : public std::runtime_error {
  CodecException(int code, const std::string &msg) 
      : std::runtime_error(msg), err_code(code)
  {} 
  
  const char* what () const noexcept override {
    std::runtime_error::what();
  }

  int ErrorCode() const noexcept { 
    return err_code; 
  } 

 private:
  int err_code;
};

}  // namespace codec

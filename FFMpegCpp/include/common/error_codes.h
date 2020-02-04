#pragma once

namespace codec {

enum ErrorCode : int {
    FAILED_ALLOC = -1,
    GENERIC_CODEC_EXCEPTION = -2,
    NULL_CODEC = -3,
    INVALID_PARAMETER = -4,
    UNEXPECTED_EXCEPTION = -5,
};

}  // namespace codec
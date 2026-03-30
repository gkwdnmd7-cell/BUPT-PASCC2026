#ifndef ERROR_CODES_H
#define ERROR_CODES_H

enum class ErrorCode {
    Ok = 0,
    Usage = 2,
    InvalidExtension = 3,
    InputNotFound = 4,
    InputUnreadable = 5,
    OutputCreateFailed = 6,
};

inline int toExitCode(ErrorCode code) {
    return static_cast<int>(code);
}

#endif

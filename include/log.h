#ifndef LOG_H
#define LOG_H

#include <iostream>
#include <string>

namespace logutil {
inline void info(const std::string& msg) {
    std::cout << "[INFO] " << msg << std::endl;
}

inline void error(const std::string& code, const std::string& msg) {
    std::cerr << "[" << code << "] " << msg << std::endl;
}
}  // namespace logutil

#endif

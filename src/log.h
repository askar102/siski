#pragma once

#include <cstdio>
#include <string>

inline void LOG(const std::string& str) {
    std::printf("%s", str.c_str());
}

template <typename... Args>
inline void LOG(const char* str, Args... args) {
    std::printf(str, args...);
}
/*
** NVFIX : Clock Recovery Service
**
** Copyright (C) 2016 Daniel Bloemendal. All rights reserved.
*/

#include "NVFIX.hpp"

void DebugPrintf(const char* fmt, ...) {
    va_list args;

    // Fetch size of formatted string
    va_start(args, fmt);
    size_t length = _vscprintf(fmt, args);
    va_end(args);

    // Format the string
    char* buffer = (char*)malloc(length + 1);
    va_start(args, fmt);
    _vsnprintf_s(buffer, length + 1, length, fmt, args);
    va_end(args);

    // Output the string
    OutputDebugStringA(buffer);
}

void DebugPrintf(const wchar_t* fmt, ...) {
    va_list args;

    // Fetch size of formatted string
    va_start(args, fmt);
    size_t length = _vscwprintf(fmt, args);
    va_end(args);

    // Format the string
    wchar_t* buffer = (wchar_t*)malloc((length + 1)*sizeof(wchar_t));
    va_start(args, fmt);
    _vsnwprintf_s(buffer, length + 1, length, fmt, args);
    va_end(args);

    // Output the string
    OutputDebugStringW(buffer);
}

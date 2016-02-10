/*
** NVFIX : Clock Recovery Service
**
** Copyright (C) 2016 Daniel Bloemendal. All rights reserved.
*/

#pragma once

#ifdef __DEBUG__
#define DEBUG_PRINT(x, ...) DebugPrintf(x, __VA_ARGS__)
#else
#define DEBUG_PRINT(x, ...) {} 
#endif

void DebugPrintf(const char* fmt, ...);
void DebugPrintf(const wchar_t* fmt, ...);

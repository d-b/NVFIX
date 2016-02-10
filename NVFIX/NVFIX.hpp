/*
** NVFIX : Clock Recovery Service
**
** Copyright (C) 2016 Daniel Bloemendal. All rights reserved.
*/

#pragma once

// Debug defines
#ifdef _DEBUG
#define __DEBUG__
#endif

// Project wide defines
#define NVFIX_TITLE    L"NVFIX Clock Recovery Service"
#define NVFIX_DESC     L"Detects abnormal clock rates on NVIDIA GPUs and performs a recovery"
#define NVFIX_SERVICE  L"NVFIX"
#define NVFIX_VERSION  L"1.0.0"
#define NVFIX_INTERVAL 60000

// Platform
#include <Windows.h>
#include <Shlwapi.h>
#include <nvapi.h>

// STL
#include <vector>
#include <string>
#include <map>
#include <algorithm>
#include <tuple>
#include <iostream>
#include <sstream>
#include <type_traits>

// Project
#include "Private.hpp"
#include "Debug.hpp"
#include "Fix.hpp"
#include "Service.hpp"
#include "Messages.h"

#pragma once

// 거의 사용되지 않는 내용을 Windows 헤더에서 제외
#define WIN32_LEAN_AND_MEAN             

#ifdef _DEBUG
#pragma comment(lib, "Debug\\ChatCore.lib")
#else
#pragma comment(lib, "Release\\ChatCore.lib")
#endif

#include "CommonPch.h"
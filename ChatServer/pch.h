#pragma once

// ���� ������ �ʴ� ������ Windows ������� ����
#define WIN32_LEAN_AND_MEAN             

#ifdef _DEBUG
#pragma comment(lib, "Debug\\ChatCore.lib")
#else
#pragma comment(lib, "Release\\ChatCore.lib")
#endif

#include "CommonPch.h"
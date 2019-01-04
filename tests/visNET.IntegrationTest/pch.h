#pragma once

#include "gtest/gtest.h"

#include "../../source/visnet.h"

#ifdef _DEBUG
#ifdef _DYNAMIC
#error Dynamic Library testing not supported
#else
#ifdef _WIN64
#pragma comment(lib, "../../bin/x64/Debug/visNET.lib")
#pragma comment(lib, "../../bin/x64/Debug/gtestd.lib")
#else
#pragma comment(lib, "../../bin/Win32/Debug/visNET.lib")
#pragma comment(lib, "../../bin/Win32/Debug/gtestd.lib")
#endif
#endif
#else
#ifdef _DYNAMIC
#error Dynamic Library testing not supported
#else
#ifdef _WIN64
#pragma comment(lib, "../../bin/x64/Release/visNET.lib")
#pragma comment(lib, "../../bin/x64/Release/gtest.lib")
#else
#pragma comment(lib, "../../bin/Win32/Release/visNET.lib")
#pragma comment(lib, "../../bin/Win32/Release/gtest.lib")
#endif
#endif
#endif

#pragma once

#include <stdint.h>

#ifdef _DEBUG
	#ifdef _WIN64
		#pragma comment(lib, "../bin/x64/Debug/visNET.lib")
	#else
		#pragma comment(lib, "../bin/Win32/Debug/visNET.lib")
	#endif
#else
	#ifdef _WIN64
		#pragma comment(lib, "../bin/x64/Release/visNET.lib")
	#else
		#pragma comment(lib, "../bin/Win32/Release/visNET.lib")
	#endif
#endif

extern "C" {
	__declspec(dllexport) bool startup();
	__declspec(dllexport) bool cleanup();
};
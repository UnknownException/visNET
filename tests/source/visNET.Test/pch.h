//
// pch.h
// Header for standard system include files.
//

#pragma once

#include "gtest/gtest.h"

#include "../../../source/visnet.h"

#ifdef _DEBUG
	#ifdef _DYNAMIC
		#error Dynamic Library testing not supported
	#else
		#ifdef _WIN64
			#pragma comment(lib, "../../../bin/x64/Debug/visNET.lib")
		#else
			#pragma comment(lib, "../../../bin/Win32/Debug/visNET.lib")
		#endif
	#endif
#else
	#ifdef _DYNAMIC
		#error Dynamic Library testing not supported
	#else
		#ifdef _WIN64
			#pragma comment(lib, "../../../bin/x64/Release/visNET.lib")
		#else
			#pragma comment(lib, "../../../bin/Win32/Release/visNET.lib")
		#endif
	#endif
#endif

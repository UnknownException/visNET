#pragma once

/*
	This UnitTest project tests all functions exposed by the visNETInterface
*/

#include <Windows.h>
#include <CppUnitTest.h>
#include "../../source/visnet.h"

#ifdef _DEBUG
	#ifdef _DYNAMIC
		#ifdef _WIN64
			#pragma comment(lib, "../../bin/x64/Debug Dynamic/visNET.lib")
		#else
			#pragma comment(lib, "../../bin/Win32/Debug Dynamic/visNET.lib")
		#endif
	#else
		#ifdef _WIN64
			#pragma comment(lib, "../../bin/x64/Debug/visNET.lib")
		#else
			#pragma comment(lib, "../../bin/Win32/Debug/visNET.lib")
		#endif
	#endif
#else
	#ifdef _DYNAMIC
		#ifdef _WIN64
			#pragma comment(lib, "../../bin/x64/Release Dynamic/visNET.lib")
		#else
			#pragma comment(lib, "../../bin/Win32/Release Dynamic/visNET.lib")
		#endif
	#else
		#ifdef _WIN64
			#pragma comment(lib, "../../bin/x64/Release/visNET.lib")
		#else
			#pragma comment(lib, "../../bin/Win32/Release/visNET.lib")
		#endif
	#endif
#endif

#pragma comment(lib, "winmm.lib")

#define TESTPACKET_PORT1 6101
#define TESTPACKET_PORT2 6102
#define TESTPACKET_PORT3 6103

#define TESTTCP_PORT1 6201
#define TESTTCP_PORT2 6202
#define TESTTCP_PORT3 6203
#define TESTTCP_PORT4 6204
#define TESTTCP_PORT5 6205
#define TESTTCP_PORT6 6206
#define TESTTCP_PORT7 6207
#define TESTTCP_PORT8 6208

#define TESTUDP_PORT1 6301
#define TESTUDP_PORT2 6302
#define TESTUDP_PORT3 6303
#define TESTUDP_PORT4 6304
#define TESTUDP_PORT5 6305
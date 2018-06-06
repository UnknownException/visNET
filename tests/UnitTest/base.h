#pragma once

/*
	This UnitTest project tests all functions exposed by the visNETInterface
*/

#include <Windows.h>
#include <CppUnitTest.h>
#include "../../interface/visNETInterface/include.h"
#ifdef _DEBUG
	#pragma comment(lib, "../../Win32/Debug/visNETInterface.lib")
#else
	#pragma comment(lib, "../../Win32/Release/visNETInterface.lib")
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
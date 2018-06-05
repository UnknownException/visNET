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

#define TESTPACKET_PORT 6101

#define TESTTCP_PORT1 6201
#define TESTTCP_PORT2 6202
#define TESTTCP_PORT3 6203
#define TESTTCP_PORT4 6204

#define TESTUDP_PORT1 6301
#define TESTUDP_PORT2 6302
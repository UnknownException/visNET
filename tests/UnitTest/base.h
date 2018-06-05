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

#define TESTPACKET_PORT 6123
#define TESTTCP_PORT1 6124
#define TESTTCP_PORT2 6125
#define TESTUDP_PORT1 6126
#define TESTUDP_PORT2 6127
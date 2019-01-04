#pragma once

#include "gtest/gtest.h"

#include "../../source/visnet.h"
#include <thread>

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


constexpr uint16_t TESTPACKET_PORT1 = 6101;
constexpr uint16_t  TESTPACKET_PORT2 = 6102;
constexpr uint16_t  TESTPACKET_PORT3 = 6103;

constexpr uint16_t  TESTTCP_PORT_CONSTRUCTCONNECTION1 = 6201;
constexpr uint16_t  TESTTCP_PORT_CONSTRUCTCONNECTION2 = 6202;
constexpr uint16_t  TESTTCP_PORT3 = 6203;
constexpr uint16_t  TESTTCP_PORT4 = 6204;
constexpr uint16_t  TESTTCP_PORT5 = 6205;
constexpr uint16_t  TESTTCP_PORT6 = 6206;
constexpr uint16_t  TESTTCP_PORT7 = 6207;
constexpr uint16_t  TESTTCP_PORT8 = 6208;

constexpr uint16_t  TESTUDP_PORT1 = 6301;
constexpr uint16_t  TESTUDP_PORT2 = 6302;
constexpr uint16_t  TESTUDP_PORT3 = 6303;
constexpr uint16_t  TESTUDP_PORT4 = 6304;
constexpr uint16_t  TESTUDP_PORT5 = 6305;
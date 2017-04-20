#pragma once

#include <iostream>
#include <stdint.h>
#include <memory>
#include <vector>
#include <atomic>
#include <mutex>
#include <thread>
#include <queue>

#ifdef _WIN32
#include <WinSock2.h>
#include <WS2tcpip.h>

#pragma comment(lib, "ws2_32.lib")
#else
//Linux libs...
#endif

// Changes memory allocation to strict (Less usage, more reallocations)
//#define _visNET_STRICT_REALLOC

#ifndef _visNET_STRICT_REALLOC
#define _visNET_LOOSE_REALLOC 1.1f // Allocate 10% above required
#endif

#define _visNET_PACKETSIZE_LIMIT 0xFFFF // Set packetsize limit to the maximum value of an unsigned short; packets bigger than this will be completely ignored
#define _visNET_NETWORKBUFFER_SIZE 0x0FFF

#include "blobarray.h"
#include "packet.h"
#include "socket.h"
#include "tcppool.h"
#include "tcplistener.h"
#include "tcpclient.h"
#include "udpclient.h"

namespace visNET{
	bool startup();
	bool cleanup();
}
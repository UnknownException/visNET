#pragma once

#include <iostream>
#include <vector>
#include <atomic>
#include <mutex>
#include <thread>
#include <queue>
#include <stdint.h>
#include <memory>

#ifdef _WIN32
#include <WinSock2.h>
#include <WS2tcpip.h>

#pragma comment(lib, "ws2_32.lib")
#else
//Linux libs...
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

namespace visNETCore{
	bool startup();
	bool cleanup();
}
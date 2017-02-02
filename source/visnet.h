#pragma once

#include <iostream>
#include <stdint.h>
#include <memory>

#include <WinSock2.h>
#include <WS2tcpip.h>
#pragma comment(lib, "ws2_32.lib")

#ifndef _visNET_EXCLUDE_BLOBARRAY
#include "blobarray.h"
#endif
#include "rawpacket.h"
#include "packet.h"
#include "socket.h"
#include "listener.h"
#include "client.h"
#ifndef _visNET_EXCLUDE_SOCKETPOOL
#include "socketpool.h"
#endif

namespace visNET{
	bool startup();
	bool cleanup();
}
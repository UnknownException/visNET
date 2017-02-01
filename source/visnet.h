#pragma once

#include <iostream>
#include <stdint.h>
#include <memory>

#include <WinSock2.h>
#include <WS2tcpip.h>
#pragma comment(lib, "ws2_32.lib")

#include "blobarray.h"
#include "rawpacket.h"
#include "packet.h"
#include "socket.h"
#include "listener.h"
#include "client.h"

namespace visNET{
	bool startup();
	bool cleanup();
}
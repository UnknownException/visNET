#pragma once

#include <iostream>
#include <stdint.h>
#include <vector>

#include <WinSock2.h>
#include <WS2tcpip.h>
#pragma comment(lib, "ws2_32.lib")

#include "blobarray.h"

namespace visNET{
	void startup();
	void cleanup();

	enum NetworkType{
		NT_TCP,
		NT_UDP,
	};
}
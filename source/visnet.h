#pragma once

#include <stdint.h>
#include <memory>
#include <vector>

#ifdef _BUILDLIBRARY
	#ifdef _WIN32
		#include <WinSock2.h>
		#include <WS2tcpip.h>

		#pragma comment(lib, "ws2_32.lib")
	#else
		// Linux
		#include <cstring>
		#include <netinet/in.h>

		#define SOCKET_ERROR -1
		#define INVALID_SOCKET -1
	#endif
#endif

#ifdef _WIN32
	#define DLLEXPORT __declspec(dllexport)
#else
	#define DLLEXPORT
#endif

#define _visNET_PACKETSIZE_LIMIT 0xFFFF // Set packetsize limit to the maximum value of an unsigned short; packets bigger than this will be completely ignored
#define _visNET_NETWORKBUFFER_SIZE 0x0FFF


#include "blobarray.h"
#include "connectionidentifier.h"
#include "packet.h"

#ifdef _BUILDLIBRARY
	#include "isocket.h"
#endif

#include "tcpmessage.h"
#ifdef _BUILDLIBRARY
	#include "tcpsocket.h"
	#include "identifiablesocket.h"
	#include "tcppool.h"
#else
	typedef void TcpPool;
	typedef void TcpSocket;
#endif
#include "tcplistener.h"
#include "tcpclient.h"

#include "udpmessage.h"
#ifdef _BUILDLIBRARY
	#include "udpsocket.h"
#else
	typedef void UdpSocket;
#endif
#include "udpclient.h"

namespace visNET{
	DLLEXPORT bool startup();
	DLLEXPORT bool cleanup();
}

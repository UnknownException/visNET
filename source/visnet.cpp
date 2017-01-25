#include "visnet.h"

namespace visNET{
	bool startup()
	{
		WSADATA wsaData;
		return (WSAStartup(MAKEWORD(2, 2), &wsaData) == NO_ERROR);
	}

	bool cleanup()
	{
		return (WSACleanup() == NO_ERROR);
	}
}
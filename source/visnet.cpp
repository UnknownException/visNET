#include "visnet.h"

namespace visNET{
	bool startup()
	{
#ifdef _WIN32
		WSADATA wsaData;
		return (WSAStartup(MAKEWORD(2, 2), &wsaData) == NO_ERROR);
#else
		return true;
#endif
	}

	bool cleanup()
	{
#ifdef _WIN32
		return (WSACleanup() == NO_ERROR);
#else
		return true;
#endif
	}
}
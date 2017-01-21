#include "visnet.h"

namespace visNET{
	void startup()
	{
		WSADATA wsaData;
		if (WSAStartup(MAKEWORD(2, 2), &wsaData) != NO_ERROR)
			throw std::exception("Failed to initialize WSA");
	}

	void cleanup()
	{
		if (!WSACleanup())
			throw std::exception("Failed to cleanup WSA");
	}
}
#include "visnet.h"
#include "udpclient.h"

namespace visNET{
	UdpClient::UdpClient(uint16_t nPort)
	{
		addrinfo hints, *result;
		ZeroMemory(&hints, sizeof(addrinfo));
		hints.ai_family = AF_INET;
		hints.ai_flags = AI_PASSIVE;
		hints.ai_socktype = SOCK_DGRAM;
		hints.ai_protocol = IPPROTO_UDP;

		result = &hints;
	
		SOCKET s = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
		if (s == INVALID_SOCKET)
		{
			setError("[UdpClient] Failed to create a socket");
			return;
		}

		sockaddr_in addr;
		addr.sin_family = AF_INET;
		addr.sin_addr.s_addr = INADDR_ANY;
		addr.sin_port = htons(nPort);

		if (bind(s, reinterpret_cast<sockaddr*>(&addr), sizeof(sockaddr_in)) < 0)
		{
			setError("[UdpListener] Failed to bind to the socket");
			return;
		}

		getSocket()->setHandle(s);

		setValid();
	}

	UdpClient::~UdpClient()
	{
	}
}
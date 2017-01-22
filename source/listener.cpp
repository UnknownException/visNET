#include "visnet.h"
#include "listener.h"

namespace visNET{
	Listener::Listener(NetworkType eType, uint16_t nPort)
	{
		addrinfo hints, *result;
		ZeroMemory(&hints, sizeof(addrinfo));
		hints.ai_family = AF_INET;
		hints.ai_flags = AI_PASSIVE;

		switch (eType)
		{
			case NT_UDP:
			{
				hints.ai_socktype = SOCK_DGRAM;
				hints.ai_protocol = IPPROTO_UDP;

				result = &hints;
			}
			break; 
			case NT_TCP:
			{
				hints.ai_socktype = SOCK_STREAM;
				hints.ai_protocol = IPPROTO_TCP;

				char szBuff[8];
				sprintf_s(szBuff, sizeof(szBuff), "%i", nPort);

				if (getaddrinfo(NULL, szBuff, &hints, &result) != 0)
					std::exception("[Listener] getaddrinfo has failed");
			}
			break;
			default:
				throw std::exception("[Listener] Invalid NetworkType specified");
		}

		SOCKET s = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
		if (s == INVALID_SOCKET)
			throw std::exception("[Listener] Failed to create socket");

		if (eType == NT_TCP)
			freeaddrinfo(result);

		sockaddr_in addr;
		addr.sin_family = AF_INET;
		addr.sin_addr.s_addr = INADDR_ANY;
		addr.sin_port = htons(nPort);

		if (bind(s, reinterpret_cast<sockaddr*>(&addr), sizeof(sockaddr_in)) < 0)
			throw std::exception("[Listener] Failed to bind the socket");

		if (eType == NT_TCP)
		{
			if (listen(s, SOMAXCONN) == SOCKET_ERROR)
				std::exception("[Listener] Failed to listen to socket");
		}

		m_handle.setSocket(s);
	}

	Listener::~Listener()
	{
	}

	Socket Listener::getConnection()
	{
		Socket s(accept(m_handle.getSocket(), nullptr, nullptr));
		if (s.getSocket() == INVALID_SOCKET)
			s.setAlive(false); //Invalid socket, set dead

		return s;
	}
}
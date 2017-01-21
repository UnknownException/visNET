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
					std::exception("[TCP Listener] getaddrinfo has failed");
			}
			break;
			default:
				throw std::exception("Invalid NetworkType specified for network listener");
		}

		SOCKET s = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
		if (s <= 0)
			throw std::exception("Failed to create listen socket");

		sockaddr_in addr;
		addr.sin_family = AF_INET;
		addr.sin_addr.s_addr = INADDR_ANY;
		addr.sin_port = htons(nPort);

		if (bind(s, reinterpret_cast<sockaddr*>(&addr), sizeof(sockaddr_in)) < 0)
			throw std::exception("Failed to bind the socket");

		if (eType == NT_TCP)
		{
			if (listen(s, SOMAXCONN) == SOCKET_ERROR)
				std::exception("Failed to listen to tcp socket");
		}

		m_handle.setSocket(s);
	}

	Listener::~Listener()
	{
	}

	bool Listener::setNonBlocking(bool b)
	{
		u_long nVal = b ? 1 : 0;

		if (ioctlsocket(m_handle.getSocket(), FIONBIO, &nVal) != 0)
			return false;

		return true;
	}

	std::vector<Socket*> Listener::getConnections()
	{
		std::vector<Socket*> vecClients;

		SOCKET s = accept(m_handle.getSocket(), nullptr, nullptr);
		while (s != INVALID_SOCKET)
		{
			vecClients.push_back(new Socket(s));

			s = accept(m_handle.getSocket(), nullptr, nullptr);
		}

		return vecClients;
	}
}
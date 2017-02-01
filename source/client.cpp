#include "visnet.h"
#include "client.h"

namespace visNET{
	Client::Client(NetworkType eType, const char* pszIp, uint16_t nPort)
	{
		m_bValid = false;

		addrinfo hints, *result;
		ZeroMemory(&hints, sizeof(addrinfo));
		hints.ai_family = AF_INET;

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

				if (getaddrinfo(pszIp, szBuff, &hints, &result) != 0)
				{
					setError("[Client] getaddrinfo has failed");
					return;
				}
			}
			break;
			default:
			{
				setError("[Client] Invalid NetworkType specified");
				return;
			}
		}

		for (auto it = result; it != nullptr; it = it->ai_next)
		{
			SOCKET s = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
			if (s == INVALID_SOCKET)
			{
				setError("[Client] Failed to create socket");
				return;
			}

			if (eType == NT_TCP)
			{
				int32_t res = connect(s, it->ai_addr, it->ai_addrlen);
				if (res == SOCKET_ERROR)
				{
					closesocket(s);
					s = INVALID_SOCKET;
					continue;
				}
			}

			m_handle.setSocket(s);

			break;
		}

		if (eType == NT_TCP)
			freeaddrinfo(result);

		if (m_handle.getSocket() == INVALID_SOCKET)
		{
			setError("[Client] Failed to connect to a socket");
			return;
		}

		m_bValid = true;
	}

	Client::~Client()
	{
	}
}
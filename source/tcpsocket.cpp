#include "visnet.h"
#include "tcpsocket.h"

namespace visNET {
	TcpSocket::TcpSocket()
	{
	}

	TcpSocket::~TcpSocket()
	{
		if(getHandle() != 0)
			closesocket(getHandle());
	}

	bool TcpSocket::listenOn(uint16_t nPort)
	{
		if (getHandle() != 0)
			return false;

		addrinfo hints, *result;
		ZeroMemory(&hints, sizeof(addrinfo));
		hints.ai_family = AF_INET;
		hints.ai_flags = AI_PASSIVE;
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_protocol = IPPROTO_TCP;

		char szBuff[8];
		sprintf_s(szBuff, sizeof(szBuff), "%i", nPort);

		if (getaddrinfo(NULL, szBuff, &hints, &result) != 0)
			return false;

		SOCKET s = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
		if (s == INVALID_SOCKET)
			return false;

		freeaddrinfo(result);

		setHandle(s);

		sockaddr_in addr;
		addr.sin_family = AF_INET;
		addr.sin_addr.s_addr = INADDR_ANY;
		addr.sin_port = htons(nPort);

		if (bind(getHandle(), reinterpret_cast<sockaddr*>(&addr), sizeof(sockaddr_in)) < 0)
			return false;

		if (listen(getHandle(), SOMAXCONN) == SOCKET_ERROR)
			return false;

		setAlive(true);
		return true;
	}

	bool TcpSocket::connectTo(const char* pszIp, uint16_t nPort)
	{
		if (getHandle() != 0)
			return false;

		addrinfo hints, *result;
		ZeroMemory(&hints, sizeof(addrinfo));
		hints.ai_family = AF_INET;
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_protocol = IPPROTO_TCP;

		char szBuff[8];
		sprintf_s(szBuff, sizeof(szBuff), "%i", nPort);

		if (getaddrinfo(pszIp, szBuff, &hints, &result) != 0)
			return false;

		for (auto it = result; it != nullptr; it = it->ai_next)
		{
			SOCKET s = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
			if (s == INVALID_SOCKET)
				return false;

			int32_t res = connect(s, it->ai_addr, it->ai_addrlen);
			if (res != SOCKET_ERROR)
			{
				setHandle(s);
				break;
			}

			closesocket(s);
		}

		freeaddrinfo(result);

		if (getHandle() != 0)
			setAlive(true);

		return getHandle() != 0;
	}

	bool TcpSocket::setNonBlocking(bool b)
	{
		if (getHandle() == 0)
			return false;

		u_long nVal = b ? 1 : 0;

		if (ioctlsocket(getHandle(), FIONBIO, &nVal) != 0)
			return false;

		return true;
	}

	bool TcpSocket::write(const uint8_t* pBuffer, int32_t nSize)
	{
		if (getHandle() == 0)
			return false;

		int32_t nRetn = send(getHandle(), reinterpret_cast<const char*>(pBuffer), nSize, 0);
		if (nRetn == SOCKET_ERROR)
			setAlive(false);

		return getAlive();
	}

	int32_t TcpSocket::read(uint8_t* pBuffer, int32_t nSize)
	{
		if (getHandle() == 0)
			return 0;

		int32_t nRetn = recv(getHandle(), reinterpret_cast<char*>(pBuffer), nSize, 0);
		if (nRetn == 0)
			setAlive(false);
		else if (nRetn > 0)
			return nRetn;

		return 0;
	}
}
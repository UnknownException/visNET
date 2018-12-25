#include "visnet.h"
#include "tcpsocket.h"

#ifndef _WIN32
	#include <unistd.h>
	#include <fcntl.h>
	#include <netdb.h>
#endif

namespace visNET {
	TcpSocket::TcpSocket()
	{
	}

	TcpSocket::~TcpSocket()
	{
#ifdef _WIN32
		if(getHandle() != 0)
			closesocket(getHandle());
#else
		if (getHandle() != 0)
			close(getHandle());
#endif
	}

	bool TcpSocket::listenOn(uint16_t nPort)
	{
		if (getHandle() != 0)
			return false;

		addrinfo hints, *result;
		memset(&hints, 0, sizeof(addrinfo));

		hints.ai_family = AF_INET;
		hints.ai_flags = AI_PASSIVE;
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_protocol = IPPROTO_TCP;

		char szBuff[8];
		snprintf(szBuff, sizeof(szBuff), "%i", nPort);

		if (getaddrinfo(NULL, szBuff, &hints, &result) != 0)
			return false;

		auto s = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
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
		memset(&hints, 0, sizeof(addrinfo));

		hints.ai_family = AF_INET;
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_protocol = IPPROTO_TCP;

		char szBuff[8];
		snprintf(szBuff, sizeof(szBuff), "%i", nPort);

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

#ifdef _WIN32
			closesocket(s);
#else
			close(s);
#endif
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

#ifdef _WIN32
		u_long nVal = b ? 1 : 0;

		if (ioctlsocket(getHandle(), FIONBIO, &nVal) != 0)
			return false;

		return true;
#else
		int32_t nFlags = fcntl(getHandle(), F_GETFL, 0);
		if (nFlags == -1)
			return false;

		nFlags = b ? nFlags | O_NONBLOCK : nFlags & ~O_NONBLOCK;
		return fcntl(getHandle(), F_SETFL, nFlags) == 0 ? true : false;
#endif
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
#include "visnet.h"
#include "udpsocket.h"

#ifndef _WIN32
	#include <unistd.h>
	#include <fcntl.h>
	#include <netdb.h>

	#include <arpa/inet.h>
#endif

namespace visNET {
	UdpSocket::UdpSocket()
	{
		addrinfo hints, *result;
		memset(&hints, 0, sizeof(addrinfo));

		hints.ai_family = AF_INET;
		hints.ai_flags = AI_PASSIVE;
		hints.ai_socktype = SOCK_DGRAM;
		hints.ai_protocol = IPPROTO_UDP;

		result = &hints;

		auto s = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
		if (s != INVALID_SOCKET)
			setHandle(s);
	}

	UdpSocket::~UdpSocket()
	{
#ifdef _WIN32
		if(getHandle() != 0)
			closesocket(getHandle());
#else
		if (getHandle() != 0)
			close(getHandle());
#endif
	}

	bool UdpSocket::bindToPort(uint16_t nPort)
	{
		if (getHandle() == 0)
			return false;

		sockaddr_in addr;
		addr.sin_family = AF_INET;
		addr.sin_addr.s_addr = INADDR_ANY;
		addr.sin_port = htons(nPort);

		if (bind(getHandle(), reinterpret_cast<sockaddr*>(&addr), sizeof(sockaddr_in)) < 0)
			return false;

		setAlive(true);
		return true;
	}

	bool UdpSocket::setNonBlocking(bool b)
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

	bool UdpSocket::write(const uint8_t* pBuffer, int32_t nSize, const char* szIP, const uint16_t nPort)
	{
		if (getHandle() == 0)
			return false;

		sockaddr_in targetAddr;
		targetAddr.sin_family = AF_INET;
		targetAddr.sin_port = htons(nPort);

#ifdef _WIN32
		InetPton(AF_INET, szIP, &targetAddr.sin_addr);

		int32_t nRes = sendto(getHandle(), reinterpret_cast<const char*>(pBuffer), nSize, 0, (SOCKADDR*)&targetAddr, sizeof(targetAddr));
#else
		inet_pton(AF_INET, szIP, &targetAddr.sin_addr);

		int32_t nRes = sendto(getHandle(), reinterpret_cast<const char*>(pBuffer), nSize, 0, (__CONST_SOCKADDR_ARG)&targetAddr, sizeof(targetAddr));
#endif
		if (nRes == SOCKET_ERROR)
			setAlive(false);

		return getAlive();
	}

	std::pair<sockaddr_in, int32_t> UdpSocket::read(uint8_t* pBuffer, int32_t nSize)
	{
		sockaddr_in senderAddr;
		memset(&senderAddr, 0, sizeof(senderAddr));

		if (getHandle() == 0)
			return std::make_pair(senderAddr, 0);

		int32_t nSenderAddrSize = sizeof(senderAddr);

#ifdef _WIN32
		int32_t nRes = recvfrom(getHandle(), reinterpret_cast<char*>(pBuffer), nSize, 0, (SOCKADDR*)&senderAddr, &nSenderAddrSize);
#else
		int32_t nRes = recvfrom(getHandle(), reinterpret_cast<char*>(pBuffer), nSize, 0, (__SOCKADDR_ARG)&senderAddr, (socklen_t*)&nSenderAddrSize);
#endif

		return std::make_pair(senderAddr, nRes);
	}
}
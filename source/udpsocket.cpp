#include "visnet.h"
#include "udpsocket.h"

namespace visNET {
	UdpSocket::UdpSocket()
	{
		addrinfo hints, *result;
		ZeroMemory(&hints, sizeof(addrinfo));
		hints.ai_family = AF_INET;
		hints.ai_flags = AI_PASSIVE;
		hints.ai_socktype = SOCK_DGRAM;
		hints.ai_protocol = IPPROTO_UDP;

		result = &hints;

		UINT_PTR s = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
		if (s != INVALID_SOCKET)
			setHandle(s);
	}

	UdpSocket::~UdpSocket()
	{
		if(getHandle() != 0)
			closesocket(getHandle());
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

		u_long nVal = b ? 1 : 0;

		if (ioctlsocket(getHandle(), FIONBIO, &nVal) != 0)
			return false;

		return true;
	}

	bool UdpSocket::write(const uint8_t* pBuffer, int32_t nSize, const char* szIP, const uint16_t nPort)
	{
		if (getHandle() == 0)
			return false;

		sockaddr_in targetAddr;
		targetAddr.sin_family = AF_INET;
		targetAddr.sin_port = htons(nPort);

		InetPton(AF_INET, szIP, &targetAddr.sin_addr);

		int32_t nRes = sendto(getHandle(), reinterpret_cast<const char*>(pBuffer), nSize, 0, (SOCKADDR*)&targetAddr, sizeof(targetAddr));
		if (nRes == SOCKET_ERROR)
			setAlive(false);

		return getAlive();
	}

	std::pair<sockaddr_in, int32_t> UdpSocket::read(uint8_t* pBuffer, int32_t nSize)
	{
//		if (getHandle() == 0)
//			return ?;

		sockaddr_in senderAddr;
		int32_t nSenderAddrSize = sizeof(senderAddr);

		int nRes = recvfrom(getHandle(), reinterpret_cast<char*>(pBuffer), nSize, 0, (SOCKADDR*)&senderAddr, &nSenderAddrSize);

		return std::make_pair(senderAddr, nRes);
	}
}
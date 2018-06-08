#include "visnet.h"
#include "socket.h"

namespace visNET{
	Socket::Socket()
	{
		m_handle = INVALID_SOCKET;
		m_bAlive = true;
	}

	Socket::~Socket()
	{
		closesocket(m_handle);
	}

	bool Socket::setNonBlocking(bool b)
	{
		u_long nVal = b ? 1 : 0;

		if (ioctlsocket(m_handle, FIONBIO, &nVal) != 0)
			return false;

		return true;
	}

	bool Socket::write(const uint8_t* pBuffer, int32_t nSize)
	{
		int32_t nRetn = send(m_handle, reinterpret_cast<const char*>(pBuffer), nSize, 0);
		if (nRetn == SOCKET_ERROR)
			m_bAlive = false;

		return m_bAlive;
	}

	int32_t Socket::read(uint8_t* pBuffer, int32_t nSize)
	{
		int32_t nRetn = recv(m_handle, reinterpret_cast<char*>(pBuffer), nSize, 0);
		if (nRetn == 0)
			m_bAlive = false;
		else if (nRetn > 0)
			return nRetn;

		return 0;
	}

	bool Socket::writeTo(const uint8_t* pBuffer, int32_t nSize, const char* szIP, const uint16_t nPort)
	{
		sockaddr_in targetAddr;
		targetAddr.sin_family = AF_INET;
		targetAddr.sin_port = htons(nPort);

		InetPton(AF_INET, szIP, &targetAddr.sin_addr);

		int32_t nRes = sendto(m_handle, reinterpret_cast<const char*>(pBuffer), nSize, 0, (SOCKADDR*)&targetAddr, sizeof(targetAddr));
		if (nRes == SOCKET_ERROR)
			m_bAlive = false;

		return m_bAlive;
	}

	std::pair<sockaddr_in, int32_t> Socket::readFrom(uint8_t* pBuffer, int32_t nSize)
	{
		sockaddr_in senderAddr;
		int32_t nSenderAddrSize = sizeof(senderAddr);

		int nRes = recvfrom(m_handle, reinterpret_cast<char*>(pBuffer), nSize, 0, (SOCKADDR*)&senderAddr, &nSenderAddrSize);

		return std::make_pair(senderAddr, nRes);
	}
}
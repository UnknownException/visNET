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
		int32_t nRetn = recv(m_handle, reinterpret_cast<char*>(pBuffer), nSize == 0 ? _visNET_NETWORKBUFFER_SIZE : nSize, 0);
		if (nRetn == 0)
			m_bAlive = false;
		else if (nRetn > 0)
			return nRetn;

		return 0;
	}
}
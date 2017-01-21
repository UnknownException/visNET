#include "visnet.h"
#include "socket.h"

namespace visNET{
	Socket::Socket()
	{
		m_bAlive = true;
	}

	Socket::~Socket()
	{
		closesocket(m_handle);
	}

	void Socket::write(const char* pBuffer, int32_t nSize)
	{
		int32_t nRetn = send(m_handle, pBuffer, nSize, 0);
		if (nRetn == SOCKET_ERROR)
			m_bAlive = false;
	}

	int32_t Socket::read(char* pBuffer, int32_t nSize)
	{
		ZeroMemory(pBuffer, nSize);
		int32_t nRetn = recv(m_handle, pBuffer, nSize, 0);
		if (nRetn == 0)
			m_bAlive = false;
		else if (nRetn > 0)
			return nRetn;
	
		return 0;
	}
}
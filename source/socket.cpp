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

	void Socket::write(const uint8_t* pBuffer, int32_t nSize)
	{
		int32_t nRetn = send(m_handle, reinterpret_cast<const char*>(pBuffer), nSize, 0);
		if (nRetn == SOCKET_ERROR)
			m_bAlive = false;
	}

	int32_t Socket::read(uint8_t* pBuffer, int32_t nSize)
	{
		ZeroMemory(pBuffer, nSize);
		int32_t nRetn = recv(m_handle, reinterpret_cast<char*>(pBuffer), nSize, 0);
		if (nRetn == 0)
			m_bAlive = false;
		else if (nRetn > 0)
			return nRetn;
	
		return 0;
	}

	void Socket::write(RawPacket& pPacket)
	{
		pPacket.onSend();
		write(pPacket.getRawData(), pPacket.getRawSize());
	}

	bool Socket::read(RawPacket& packet)
	{
		if (packet.getRawSize() != 0)
			throw std::exception("Cannot read data into an already filled packet");

		uint8_t buff[512]; // todo : Make buffer a configurable setting
		int32_t nSize = read(buff, sizeof(buff));
		if (nSize <= 0)
			return false;

		packet.onReceive(buff, nSize);
			
		return true;
	}
}
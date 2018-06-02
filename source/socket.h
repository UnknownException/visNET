#pragma once

namespace visNETCore{
	class Socket{
		SOCKET m_handle;
		bool m_bAlive;
	public:
		Socket();
		virtual ~Socket();

		void setHandle(SOCKET s) { m_handle = s; }
		SOCKET getHandle() { return m_handle; }

		bool setNonBlocking(bool b);

		void setAlive(bool b) { m_bAlive = b; }
		bool getAlive() { return m_bAlive; }

		bool write(const uint8_t* pBuffer, int32_t nSize);
		int32_t read(uint8_t* pBuffer, int32_t nSize);

		bool writeTo(const uint8_t* pBuffer, int32_t nSize, const char* szIP, const uint16_t nPort);
		std::pair<sockaddr_in, int32_t> readFrom(uint8_t* pBuffer, int32_t nSize);
	};
}
#pragma once

namespace visNET{
	class Socket{
		SOCKET m_handle;
		bool m_bAlive;
	public:
		Socket();
		virtual ~Socket();

		void setSocket(SOCKET s) { m_handle = s; }
		SOCKET getSocket() { return m_handle; }

		bool setNonBlocking(bool b);

		void setAlive(bool b) { m_bAlive = b; }
		bool getAlive() { return m_bAlive; }

		bool write(const uint8_t* buffer, int32_t size);
		bool write(RawPacket& packet);
		int32_t read(uint8_t* buffer, int32_t size);
		bool read(RawPacket& packet);
	};
}
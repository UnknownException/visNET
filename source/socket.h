#pragma once

namespace visNET{
	class Socket{
		SOCKET m_handle;
		bool m_bAlive;
	public:
		Socket();
		virtual ~Socket();

		Socket(SOCKET s) {
			Socket();
			setSocket(s);
		}

		void setSocket(SOCKET s) { m_handle = s; }
		SOCKET getSocket() { return m_handle; }

		bool getAlive() { return m_bAlive; }

		void write(const uint8_t* buffer, int32_t size);
		void write(RawPacket& packet);
		int32_t read(uint8_t* buffer, int32_t size);
		bool read(RawPacket& packet);
	};
}
#pragma once

namespace visNET{
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

		bool write(const uint8_t* buffer, int32_t size);
		int32_t read(uint8_t* buffer, int32_t size = 0);
	private:

	};
}
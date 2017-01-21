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

		void setSocket(SOCKET s){
			m_handle = s;
		}

		SOCKET getSocket() {
			return m_handle;
		}

		void write(const char* buffer, int32_t size);
		int32_t read(char* buffer, int32_t size);

		bool getAlive() {
			return m_bAlive;
		}
	};
}
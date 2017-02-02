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

		bool write(RawPacket& packet);
		template <typename T>
		bool write(const T* buffer, int32_t size) {
			return _write(reinterpret_cast<const char*>(buffer), size);
		}

		bool read(RawPacket& packet);
		template <typename T>
		int32_t read(T* buffer, int32_t size) {
			return _read(reinterpret_cast<char*>(buffer), size);
		}
	private:
		bool _write(const char* buffer, int32_t size);
		int32_t _read(char* buffer, int32_t size);
	};
}
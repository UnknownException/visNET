#pragma once

/*
	Client is the client sided connection
	This class will connect to a listener
*/

namespace visNET{
	class Client{
		Socket m_handle;
	public:
		Client(NetworkType eType, const char* pszIp, uint16_t nPort);
		virtual ~Client();

		void write(const uint8_t* buffer, int32_t size) { m_handle.write(buffer, size); }
		void write(RawPacket& packet) { m_handle.write(packet); }
		int32_t read(uint8_t* buffer, int32_t size) { return m_handle.read(buffer, size); }
		bool read(RawPacket& packet) { return m_handle.read(packet); }
	};
}
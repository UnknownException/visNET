#pragma once

/*
	Client is the client sided connection
	This class will connect to a listener
*/

namespace visNET{
	class Client{
		Socket m_handle;
		bool m_bValid; // If not valid, the Listener hasn't been created.
		std::string m_strError;
	public:
		Client(NetworkType eType, const char* pszIp, uint16_t nPort);
		virtual ~Client();

		bool write(const uint8_t* buffer, int32_t size) { return m_handle.write(buffer, size); }
		bool write(RawPacket& packet) { return m_handle.write(packet); }
		int32_t read(uint8_t* buffer, int32_t size) { return m_handle.read(buffer, size); }
		bool read(RawPacket& packet) { return m_handle.read(packet); }

		bool isValid() { return m_bValid; }
		const std::string getError() { return m_strError; }
	private:
		void setError(const char* pszError) { m_strError = pszError; }
	};
}
#pragma once

/*
	Listener class is the server sided 'listener' which will wait for new client connections
*/

namespace visNET{
	enum NetworkType{
		NT_TCP,
		NT_UDP,
	};

	class Listener{
		Socket m_handle;
		bool m_bValid; // If not valid, the Listener hasn't been created.
		NetworkType m_eType;
		std::string m_strError;
	public:
		Listener(NetworkType eType, uint16_t nPort);
		virtual ~Listener();

		bool setNonBlocking(bool b) { return m_handle.setNonBlocking(b); }
		std::unique_ptr<Socket> getConnection();

		bool isValid() { return m_bValid; }
		const std::string getError() { return m_strError; }
	private:
		void setError(const char* pszError) { m_strError = pszError; }
	};
}
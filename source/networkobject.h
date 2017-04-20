#pragma once

namespace visNET{
	class NetworkObject{
		Socket m_socket;
		bool m_bValid;
		std::string m_strError;
	public:
		NetworkObject() { m_bValid = false; }
		virtual ~NetworkObject() {};

		bool isValid() { return m_bValid; }
	protected:
		void setValid() { m_bValid = true; }

	public:
		Socket* getSocket() { return &m_socket; }
	protected:
		void setSocket(Socket& s) { m_socket = s; }

	public:
		const std::string getError() { return m_strError; }
	protected:
		void setError(const char* pszError) { m_strError = pszError; }

	public:
		bool setNonBlocking(bool b) { return getSocket()->setNonBlocking(b); }
	};
}
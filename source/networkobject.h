#pragma once

namespace visNET{
	class __declspec(dllexport) NetworkObject{
#ifdef _BUILDLIBRARY
		Socket* m_socket;
		std::string* m_strError;
#else
		void* m_socket;
		void* m_strError;
#endif

		bool m_bValid;

	public:
		NetworkObject();
		virtual ~NetworkObject();

		bool isValid() { return m_bValid; }
	protected:
		void setValid() { m_bValid = true; }

#ifdef _BUILDLIBRARY
		Socket* getSocket() { return m_socket; }
#else
		void* getSocket() { return m_socket; }
#endif

	public:
		const std::string getError();
	protected:
		void setError(const char* pszError);

	protected:
		bool setNonBlocking(bool b);
	};
}
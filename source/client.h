#pragma once

namespace visNET{
	class Client{
		Socket m_handle;
	public:
		Client(NetworkType eType, const char* pszIp, uint16_t nPort);
		virtual ~Client();
	};
}
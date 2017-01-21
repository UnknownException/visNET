#pragma once

#include "socket.h"

namespace visNET{
	class Listener{
		Socket m_handle;
	public:
		Listener(NetworkType eType, uint16_t nPort);
		virtual ~Listener();

		bool setNonBlocking(bool b);
		std::vector<Socket*> getConnections();
	};
}
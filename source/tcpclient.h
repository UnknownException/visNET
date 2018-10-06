#pragma once

#include "networkobject.h"

/*
	Client is the client sided connection
	This class will connect to a listener
*/

namespace visNET{
	class __declspec(dllexport) TcpClient : public NetworkObject{
#ifdef _BUILDLIBRARY
		TcpPool* m_pTcpPool;
#else
		void* m_pTcpPool;
#endif
		ConnectionIdentifier m_serverIdentifier;

	public:
		TcpClient(const char* pszIp, uint16_t nPort);
		virtual ~TcpClient();

		void send(std::shared_ptr<Packet> pPacket);
		void send(Packet& pPacket);
		std::vector<TcpMessage> receive();
		bool isDisconnected();
	};
}
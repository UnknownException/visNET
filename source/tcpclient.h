#pragma once

#include "networkobject.h"

/*
	Client is the client sided connection
	This class will connect to a listener
*/

namespace visNETCore{
	class TcpClient : public NetworkObject{
		TcpPool* m_pTcpPool;
		ConnectionIdentifier m_serverIdentifier;

	public:
		TcpClient(const char* pszIp, uint16_t nPort);
		virtual ~TcpClient();

		void send(std::shared_ptr<Packet> pPacket);
		std::vector<TcpMessage> getPackets();
		bool isDisconnected();
	};
}
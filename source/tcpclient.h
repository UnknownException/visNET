#pragma once

#include "networkobject.h"

/*
	Client is the client sided connection
	This class will connect to a listener
*/

namespace visNETCore{
	class TcpClient : public NetworkObject{
		TcpPool* m_pTcpPool;
		uint32_t m_nServerID;

	public:
		TcpClient(const char* pszIp, uint16_t nPort);
		virtual ~TcpClient();

		void send(std::shared_ptr<Packet> pPacket) { m_pTcpPool->sendPacket(m_nServerID, pPacket); }
		std::vector<std::pair<uint32_t, std::shared_ptr<Packet>>> getPackets() { return m_pTcpPool->getPackets(); }
		bool isDisconnected() { return !m_pTcpPool->getDisconnected(true).empty(); }
	};
}
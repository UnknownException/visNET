#pragma once
#include "networkobject.h"

namespace visNET{
	class TcpListener : public NetworkObject{
		TcpPool* m_pTcpPool;

	public:
		TcpListener(uint16_t nPort);
		virtual ~TcpListener();

		uint32_t getConnection();

		void send(uint32_t nClId, std::shared_ptr<Packet> pPacket) { m_pTcpPool->sendPacket(nClId, pPacket); }
		std::vector<std::pair<uint32_t, std::shared_ptr<Packet>>> getPackets() { return m_pTcpPool->getPackets(); }
	};
}
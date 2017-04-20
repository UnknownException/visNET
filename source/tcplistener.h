#pragma once
#include "networkobject.h"

namespace visNET{
	class TcpListener : public NetworkObject{
		TcpPool* m_pTcpPool;

	public:
		TcpListener(uint16_t nPort);
		virtual ~TcpListener();

		uint32_t getConnection();

		void send(uint32_t nClId, Packet& packet) { m_pTcpPool->sendPacket(nClId, std::make_shared<visNET::Packet>(packet)); }
		std::vector<std::pair<uint32_t, std::shared_ptr<Packet>>> getPackets() { return m_pTcpPool->getPackets(); }
	};
}
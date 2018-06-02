#pragma once
#include "networkobject.h"

namespace visNETCore{
	class TcpListener : public NetworkObject{
		TcpPool* m_pTcpPool;

	public:
		TcpListener(uint16_t nPort);
		virtual ~TcpListener();

		uint32_t getConnection();

		void send(uint32_t nClId, std::shared_ptr<Packet> pPacket) { m_pTcpPool->sendPacket(nClId, pPacket); }
		std::vector<std::pair<uint32_t, std::shared_ptr<Packet>>> getPackets() { return m_pTcpPool->getPackets(); }
		std::vector<uint32_t> getDisconnected() { return m_pTcpPool->getDisconnected(false); }
		void disconnect(uint32_t nClId) { m_pTcpPool->removeSocket(nClId); }
	};
}
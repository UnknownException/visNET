#pragma once
#include "networkobject.h"

namespace visNETCore{
	class TcpListener : public NetworkObject{
		TcpPool* m_pTcpPool;

	public:
		TcpListener(uint16_t nPort);
		virtual ~TcpListener();

		uint32_t getConnection();

		void send(uint32_t nClId, std::shared_ptr<Packet> pPacket) { if (isValid()) { m_pTcpPool->sendPacket(nClId, pPacket); } }
		std::vector<std::pair<uint32_t, std::shared_ptr<Packet>>> getPackets() { return isValid() ? m_pTcpPool->getPackets() : std::vector<std::pair<uint32_t, std::shared_ptr<Packet>>>(); }
		std::vector<uint32_t> getDisconnected() { return isValid() ? m_pTcpPool->getDisconnected(false) : std::vector<uint32_t>(); }
		void disconnect(uint32_t nClId) { if (isValid()) { m_pTcpPool->removeSocket(nClId); } }
	};
}
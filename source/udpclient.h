#pragma once
#include "networkobject.h"

namespace visNETCore{
	class UdpClient : public NetworkObject{
	public:
		UdpClient(uint16_t nPort);
		virtual ~UdpClient();

		bool send(std::shared_ptr<Packet> pPacket, const char* szIP, unsigned short nPort) { pPacket->_onSend(); return getSocket()->writeTo(pPacket->_getRawData(), pPacket->_getRawSize(), szIP, nPort); }
		std::vector<std::tuple<std::string, uint16_t, std::shared_ptr<Packet>>> getPackets();
	};
}
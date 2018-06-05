#pragma once
#include "networkobject.h"

namespace visNETCore{
	class UdpClient : public NetworkObject{
		uint8_t* m_pBuffer;
	public:
		UdpClient(uint16_t nPort);
		virtual ~UdpClient();

		bool send(UdpMessage& message);
		std::vector<UdpMessage> getPackets();
	};
}
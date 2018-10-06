#pragma once
#include "networkobject.h"

namespace visNET{
	class __declspec(dllexport) UdpClient : public NetworkObject{
		uint8_t* m_pBuffer;
	public:
		UdpClient(uint16_t nPort);
		virtual ~UdpClient();

		bool send(UdpMessage& message);
		bool send(std::string ip, uint16_t port, Packet& packet);

		std::vector<UdpMessage> receive();
	private:
		bool validatePacket(uint8_t* buffer, uint32_t bufferSize, uint32_t expectedPacketSize);
		uint32_t generateChecksum(const uint8_t* buffer, const uint32_t bufferSize);
	};
}
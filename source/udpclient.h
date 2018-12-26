#pragma once

namespace visNET{
	class DLLEXPORT UdpClient {
		UdpSocket* m_pSocket;
		uint8_t* m_pBuffer;
		bool m_bValid;
	public:
		UdpClient(uint16_t nPort);
		virtual ~UdpClient();

		bool send(UdpMessage& message);
		bool send(std::string ip, uint16_t port, Packet& packet);

		std::vector<UdpMessage> receive();

		bool isValid() { return m_bValid; }
	private:
		bool validatePacket(uint8_t* buffer, uint32_t bufferSize, uint32_t expectedPacketSize);
		uint32_t generateChecksum(const uint8_t* buffer, const uint32_t bufferSize);
	};
}
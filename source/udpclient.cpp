#include "visnet.h"
#include "udpclient.h"

#ifndef _WIN32
	#include <arpa/inet.h>
#endif

namespace visNET{
	UdpClient::UdpClient(uint16_t nPort)
	{
		m_bValid = false;

		// Reserve buffer for one full packet and checksum
		m_pBuffer = new uint8_t[_visNET_PACKETSIZE_LIMIT + sizeof(uint32_t)];
		m_pSocket = new UdpSocket();
		if (!m_pSocket->bindToPort(nPort))
			return;

		m_pSocket->setNonBlocking(true);
		m_bValid = true;
	}

	UdpClient::~UdpClient()
	{
		delete[] m_pBuffer;
		delete m_pSocket;
	}

	bool UdpClient::send(UdpMessage& message) 
	{
		if (!isValid() || !message.getPacket())
			return false;

		message.getPacket()->_onSend(); 

		uint32_t checksum = generateChecksum(message.getPacket()->_getRawData(), message.getPacket()->_getRawSize());
		memcpy(m_pBuffer, &checksum, sizeof(uint32_t));
			
		memcpy(m_pBuffer + sizeof(uint32_t), message.getPacket()->_getRawData(), message.getPacket()->_getRawSize());
		return m_pSocket->write(m_pBuffer, message.getPacket()->_getRawSize() + sizeof(uint32_t), message.getIP().c_str(), message.getPort());
	}

	bool UdpClient::send(std::string ip, uint16_t port, Packet& packet)
	{
		UdpMessage message(ip, port, packet._copy());
		return send(message);
	}

	std::vector<UdpMessage> UdpClient::receive()
	{
		if (!isValid())
			return std::vector<UdpMessage>();

		std::vector<UdpMessage> retn;

		auto result = m_pSocket->read(m_pBuffer, _visNET_PACKETSIZE_LIMIT);
		while (result.second > 0)
		{
			// Check if packet atleast contains checksum and packetsize
			if (static_cast<size_t>(result.second) >= sizeof(uint32_t) + sizeof(uint32_t))
			{
				auto packet = std::make_shared<Packet>();
				uint32_t size = 0;
				memcpy(&size, m_pBuffer + sizeof(uint32_t), sizeof(uint32_t));

				if (validatePacket(m_pBuffer, result.second, size))
				{
					if (packet->_onReceive(m_pBuffer + sizeof(uint32_t), size) == 0)
					{
						char tmp[INET_ADDRSTRLEN];

#ifdef _WIN32
						InetNtop(AF_INET, &result.first.sin_addr, tmp, INET_ADDRSTRLEN);
#else
						inet_ntop(AF_INET, &result.first.sin_addr, tmp, INET_ADDRSTRLEN);
#endif
						std::string address(tmp);
						UdpMessage message(address, result.first.sin_port, packet);
						retn.push_back(message);
					}
				}
			}

			result = m_pSocket->read(m_pBuffer, _visNET_PACKETSIZE_LIMIT);
		}

		return retn;
	}

	bool UdpClient::validatePacket(uint8_t* buffer, uint32_t bufferSize, uint32_t expectedPacketSize)
	{
		// Check if received buffersize minus checksum is same as packetsize defined in packet
		if (expectedPacketSize != bufferSize - sizeof(uint32_t))
			return false;

		uint32_t checksum = 0;
		memcpy(&checksum, buffer, sizeof(uint32_t));

		return (checksum == generateChecksum(buffer + sizeof(uint32_t), expectedPacketSize));
	}

	uint32_t UdpClient::generateChecksum(const uint8_t* buffer, const uint32_t bufferSize)
	{
		uint32_t checksum = 0;

		for (uint32_t i = 0; i < bufferSize; ++i)
			checksum += buffer[i] ^ (i % 256);

		return checksum ^ 0x1234FEDC;
	}
}
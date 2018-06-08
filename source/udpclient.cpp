#include "visnet.h"
#include "udpclient.h"

namespace visNETCore{
	UdpClient::UdpClient(uint16_t nPort)
	{
		// Reserve buffer for one full packet and checksum
		m_pBuffer = new uint8_t[_visNET_PACKETSIZE_LIMIT + sizeof(uint32_t)];

		addrinfo hints, *result;
		ZeroMemory(&hints, sizeof(addrinfo));
		hints.ai_family = AF_INET;
		hints.ai_flags = AI_PASSIVE;
		hints.ai_socktype = SOCK_DGRAM;
		hints.ai_protocol = IPPROTO_UDP;

		result = &hints;
	
		SOCKET s = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
		if (s == INVALID_SOCKET)
		{
			setError("[UdpClient] Failed to create a socket");
			return;
		}

		sockaddr_in addr;
		addr.sin_family = AF_INET;
		addr.sin_addr.s_addr = INADDR_ANY;
		addr.sin_port = htons(nPort);

		if (bind(s, reinterpret_cast<sockaddr*>(&addr), sizeof(sockaddr_in)) < 0)
		{
			setError("[UdpListener] Failed to bind to the socket");
			return;
		}

		getSocket()->setHandle(s);
		getSocket()->setNonBlocking(true);

		setValid();
	}

	UdpClient::~UdpClient()
	{
		delete[] m_pBuffer;
	}

	bool UdpClient::send(UdpMessage& message) 
	{
		if (!isValid() || !message.getPacket())
			return false;

		message.getPacket()->_onSend(); 

		uint32_t checksum = generateChecksum(message.getPacket()->_getRawData(), message.getPacket()->_getRawSize());
		memcpy(m_pBuffer, &checksum, sizeof(uint32_t));
			
		memcpy(m_pBuffer + sizeof(uint32_t), message.getPacket()->_getRawData(), message.getPacket()->_getRawSize());
		return getSocket()->writeTo(m_pBuffer, message.getPacket()->_getRawSize() + sizeof(uint32_t), message.getIP().c_str(), message.getPort());
	}

	std::vector<UdpMessage> UdpClient::getPackets()
	{
		if (!isValid())
			return std::vector<UdpMessage>();

		std::vector<UdpMessage> retn;

		auto result = getSocket()->readFrom(m_pBuffer, _visNET_PACKETSIZE_LIMIT);
		while (result.second > 0)
		{
			// Check if packet atleast contains checksum and packetsize
			if (result.second >= sizeof(uint32_t) + sizeof(uint32_t))
			{
				auto packet = std::make_shared<Packet>();
				uint32_t size = 0;
				memcpy(&size, m_pBuffer + sizeof(uint32_t), sizeof(uint32_t));

				if (validatePacket(m_pBuffer, result.second, size))
				{
					if (packet->_onReceive(m_pBuffer + sizeof(uint32_t), size) == 0)
					{
						char tmp[INET_ADDRSTRLEN];
						InetNtop(AF_INET, &result.first.sin_addr, tmp, INET_ADDRSTRLEN);

						retn.push_back(UdpMessage(std::string(tmp), result.first.sin_port, packet));
					}
				}
			}

			result = getSocket()->readFrom(m_pBuffer, _visNET_PACKETSIZE_LIMIT);
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
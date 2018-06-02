#include "visnet.h"
#include "udpclient.h"

namespace visNETCore{
	UdpClient::UdpClient(uint16_t nPort)
	{
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
	}

	std::vector<std::tuple<std::string, uint16_t, std::shared_ptr<Packet>>> UdpClient::getPackets()
	{
		std::vector<std::tuple<std::string, uint16_t, std::shared_ptr<Packet>>> retn;

		uint8_t* buffer = new uint8_t[_visNET_NETWORKBUFFER_SIZE];
		auto result = getSocket()->readFrom(buffer, _visNET_NETWORKBUFFER_SIZE);
		while (result.second > 0)
		{
			if (result.second >= sizeof(uint32_t))
			{
				auto packet = std::make_shared<Packet>();
				uint32_t nSize = 0;
				memcpy(&nSize, buffer, sizeof(nSize));
				if (nSize == result.second)
				{
					packet->_onReceive(buffer, result.second);
					
					char tmp[INET_ADDRSTRLEN];
					InetNtop(AF_INET, &result.first.sin_addr, tmp, INET_ADDRSTRLEN);

					retn.push_back(std::make_tuple(std::string(tmp), result.first.sin_port, packet));
				}
				else
				{
					printf("Failed, packetsize not equal\n");
				}
			}

			result = getSocket()->readFrom(buffer, _visNET_NETWORKBUFFER_SIZE);
		}

		return retn;
	}
}
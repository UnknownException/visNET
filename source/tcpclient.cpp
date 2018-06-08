#include "visnet.h"
#include "tcpclient.h"

namespace visNET{
	TcpClient::TcpClient(const char* pszIp, uint16_t nPort)
	{
		m_pTcpPool = nullptr;

		addrinfo hints, *result;
		ZeroMemory(&hints, sizeof(addrinfo));
		hints.ai_family = AF_INET;
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_protocol = IPPROTO_TCP;

		char szBuff[8];
		sprintf_s(szBuff, sizeof(szBuff), "%i", nPort);

		if (getaddrinfo(pszIp, szBuff, &hints, &result) != 0)
		{
			setError("[TcpClient] getaddrinfo has failed");
			return;
		}

		for (auto it = result; it != nullptr; it = it->ai_next)
		{
			SOCKET s = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
			if (s == INVALID_SOCKET)
			{
				setError("[TcpClient] Failed to create socket");
				return;
			}

			int32_t res = connect(s, it->ai_addr, it->ai_addrlen);
			if (res == SOCKET_ERROR)
			{
				closesocket(s);
				s = INVALID_SOCKET;
				continue;
			}

			getSocket()->setHandle(s);
			getSocket()->setNonBlocking(true);

			break;
		}

		freeaddrinfo(result);

		if (getSocket()->getHandle() == INVALID_SOCKET)
		{
			setError("[Client] Failed to connect to a socket");
			return;
		}

		m_pTcpPool = new TcpPool;
		m_pTcpPool->SetKeepHistory(true);

		m_serverIdentifier = m_pTcpPool->addSocket(std::make_shared<Socket>(*getSocket()));

		setValid();
	}

	TcpClient::~TcpClient()
	{
		if (m_pTcpPool)
			delete m_pTcpPool;
	}

	void TcpClient::send(std::shared_ptr<Packet> pPacket) 
	{ 
		if (!isValid())
			return; 
		
		m_pTcpPool->sendPacket(TcpMessage(m_serverIdentifier, pPacket));
	}

	void TcpClient::send(Packet& packet)
	{
		send(packet._copy());
	}

	std::vector<TcpMessage> TcpClient::getPackets()
	{
		if (!isValid())
			return std::vector<TcpMessage>();

		return m_pTcpPool->getPackets();
	}

	bool TcpClient::isDisconnected() 
	{ 
		if (!isValid())
			return true;

		return !m_pTcpPool->getDisconnected().empty();
	}
}
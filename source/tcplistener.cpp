#include "visnet.h"
#include "tcplistener.h"

namespace visNET{
	TcpListener::TcpListener(uint16_t nPort)
	{
		m_pTcpPool = nullptr;

		addrinfo hints, *result;
		ZeroMemory(&hints, sizeof(addrinfo));
		hints.ai_family = AF_INET;
		hints.ai_flags = AI_PASSIVE;

		hints.ai_socktype = SOCK_STREAM;
		hints.ai_protocol = IPPROTO_TCP;

		char szBuff[8];
		sprintf_s(szBuff, sizeof(szBuff), "%i", nPort);

		if (getaddrinfo(NULL, szBuff, &hints, &result) != 0)
		{
			setError("[TcpListener] Failed to get address info");
			return;
		}

		SOCKET s = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
		if (s == INVALID_SOCKET)
		{
			setError("[TcpListener] Failed to create a socket");
			return;
		}

		freeaddrinfo(result);

		sockaddr_in addr;
		addr.sin_family = AF_INET;
		addr.sin_addr.s_addr = INADDR_ANY;
		addr.sin_port = htons(nPort);

		if (bind(s, reinterpret_cast<sockaddr*>(&addr), sizeof(sockaddr_in)) < 0)
		{
			setError("[TcpListener] Failed to bind to the socket");
			return;
		}

		if (listen(s, SOMAXCONN) == SOCKET_ERROR)
		{
			setError("[TcpListener] Failed to listen to the socket");
			return;
		}

		getSocket()->setHandle(s);
		getSocket()->setNonBlocking(true);

		m_pTcpPool = new TcpPool;

		setValid();
	}

	TcpListener::~TcpListener()
	{
		if (m_pTcpPool)
			delete m_pTcpPool;
	}

	ConnectionIdentifier TcpListener::getConnection()
	{
		if (!isValid())
			return ConnectionIdentifier();

		std::shared_ptr<Socket> s = std::make_shared<Socket>();
		s->setHandle(accept(getSocket()->getHandle(), nullptr, nullptr));
		if (s->getHandle() == INVALID_SOCKET)
			return ConnectionIdentifier();

		s->setNonBlocking(true);

		return m_pTcpPool->addSocket(s);
	}

	void TcpListener::send(TcpMessage& message) 
	{ 
		if (!isValid())
			return;

		m_pTcpPool->sendPacket(message); 
	}

	void TcpListener::send(ConnectionIdentifier identifier, Packet& packet)
	{
		send(TcpMessage(identifier, packet._copy()));
	}
	
	std::vector<TcpMessage> TcpListener::getPackets() 
	{ 
		if(!isValid())
			return std::vector<TcpMessage>();

		return m_pTcpPool->getPackets();
	}
	
	std::vector<ConnectionIdentifier> TcpListener::getDisconnected()
	{ 
		if (!isValid())
			return  std::vector<ConnectionIdentifier>();

		return m_pTcpPool->getDisconnected();
	}
	
	void TcpListener::disconnect(ConnectionIdentifier clientId) 
	{ 
		if (!isValid())
			return;

		m_pTcpPool->removeSocket(clientId);
	}
}
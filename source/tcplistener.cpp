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

		setValid();

		m_pTcpPool = new TcpPool;
	}

	TcpListener::~TcpListener()
	{
		if (m_pTcpPool)
			delete m_pTcpPool;
	}

	uint32_t TcpListener::getConnection()
	{
		std::shared_ptr<Socket> s = std::make_shared<Socket>();
		s->setHandle(accept(getSocket()->getHandle(), nullptr, nullptr));
		if (s->getHandle() == INVALID_SOCKET)
			s->setAlive(false);

		return m_pTcpPool->addSocket(s);
	}
}
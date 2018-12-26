#include "visnet.h"
#include "tcplistener.h"

namespace visNET{
	TcpListener::TcpListener(uint16_t nPort)
	{
		m_pTcpPool = nullptr;
		m_bValid = false;

		m_pSocket = new TcpSocket();
		if (!m_pSocket->listenOn(nPort))
			return;

		m_pSocket->setNonBlocking(true);

		m_pTcpPool = new TcpPool;
	
		m_bValid = true;
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

		std::shared_ptr<TcpSocket> s = std::make_shared<TcpSocket>();
		s->setHandle(accept(m_pSocket->getHandle(), nullptr, nullptr));
		if (s->getHandle() == INVALID_SOCKET)
			return ConnectionIdentifier();

		s->setAlive(true);
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
		TcpMessage message(identifier, packet._copy());
		send(message);
	}
	
	std::vector<TcpMessage> TcpListener::receive() 
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
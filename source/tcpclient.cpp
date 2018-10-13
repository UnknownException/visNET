#include "visnet.h"
#include "tcpclient.h"

namespace visNET{
	TcpClient::TcpClient(const char* pszIp, uint16_t nPort)
	{
		m_pTcpPool = nullptr;
		m_bValid = false;

		auto pSocket = std::make_shared<TcpSocket>();
		if (!pSocket->connectTo(pszIp, nPort))
			return;

		pSocket->setNonBlocking(true);

		m_pTcpPool = new TcpPool;
		m_pTcpPool->SetKeepHistory(true);

		m_serverIdentifier = m_pTcpPool->addSocket(pSocket);

		m_bValid = true;;
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

	std::vector<TcpMessage> TcpClient::receive()
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
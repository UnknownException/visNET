#include "visnet.h"
#include "tcppool.h"

namespace visNETCore{
	TcpPool::TcpPool()
	{
		m_bPacketsAvailable = false;
		m_bRun = true;

		m_currentIdentifier.increase(); // Start at 1

		m_pBuffer = new uint8_t[_visNET_NETWORKBUFFER_SIZE];

		m_pThread = std::make_unique<std::thread>(&TcpPool::run, this);
	}

	TcpPool::~TcpPool()
	{
		m_bRun = false;
		m_pThread->join();

		delete[] m_pBuffer;
	}

	ConnectionIdentifier TcpPool::generateIdentifier()
	{
		ConnectionIdentifier freeIdentifier;

		m_mutReserveID.lock();

			if (m_queReserveID.empty())
			{
				freeIdentifier = m_currentIdentifier;
				m_currentIdentifier.increase();
			}
			else
			{
				freeIdentifier = m_queReserveID.front();
				m_queReserveID.pop();
			}

		m_mutReserveID.unlock();

		return freeIdentifier;
	}

	ConnectionIdentifier TcpPool::addSocket(std::shared_ptr<Socket> s)
	{
		auto id = generateIdentifier();

		m_mutNewSockets.lock();
		m_vecNewSockets.push_back(std::make_pair(id, s));
		m_mutNewSockets.unlock();

		return id;
	}

	void TcpPool::removeSocket(ConnectionIdentifier connectionId)
	{
		m_mutDisconnectSockets.lock();

		if (std::find(m_vecDisconnectSockets.begin(), m_vecDisconnectSockets.end(), connectionId) == m_vecDisconnectSockets.end())
			m_vecDisconnectSockets.push_back(connectionId);

		m_mutDisconnectSockets.unlock();
	}

	std::vector<TcpMessage> TcpPool::getPackets()
	{
		std::vector<TcpMessage> packets;

		if (!m_bPacketsAvailable)
			return packets;

		m_mutRecvPackets.lock();
		packets.swap(m_vecRecvPackets);
		m_mutRecvPackets.unlock();

		m_bPacketsAvailable = false;

		return packets;
	}

	void TcpPool::sendPacket(TcpMessage& message)
	{
		m_mutSendPackets.lock();
		m_vecSendPackets.push_back(message);
		m_mutSendPackets.unlock();
	}
	
	std::vector<ConnectionIdentifier> TcpPool::getDisconnected(bool bKeepHistory)
	{
		/* 
			Create a list of disconnected ID's
		*/

		m_mutDisconnected.lock();

		auto retn(m_vecDisconnected);

		if(!bKeepHistory)
			m_vecDisconnected.clear();
	
		m_mutDisconnected.unlock();

		// ID's are not free; Return
		if (bKeepHistory)
			return retn;

		/*
			Application will be notified with disconnected ID's
			Add the ID's back to the usable ID pool
		*/
		m_mutReserveID.lock();  
		
		for (auto it = retn.begin(); it != retn.end(); ++it)
			m_queReserveID.push(*it);

		m_mutReserveID.unlock();

		return retn;
	}

	void TcpPool::run()
	{
		while (m_bRun)
		{
			acceptConnections();

			decltype(m_vecRecvPackets) packets;
			for (auto it = m_vecSockets.begin(); it != m_vecSockets.end();)
			{
				auto nId = (*it).first;
				auto pSocket = (*it).second;

				auto pReceived = recvPackets(nId, pSocket);
				packets.insert(packets.end(), pReceived.begin(), pReceived.end());

				sendPackets(nId, pSocket);

				if(cleanupConnection(nId, pSocket))
					it = m_vecSockets.erase(it);
				else
					it++;
			}

			/* 
				Move packets to the storage buffer
			*/
			m_mutRecvPackets.lock();
			if (!packets.empty())
				m_vecRecvPackets.insert(m_vecRecvPackets.end(), packets.begin(), packets.end());

			m_bPacketsAvailable = !m_vecRecvPackets.empty();
			m_mutRecvPackets.unlock();

			/*
				Pause when there are no packets to be send and none in transmission
				If the lock fails, skip this step commpletely
			*/
			if (m_mutSendPackets.try_lock())
			{
				bool bEmpty = m_vecSendPackets.empty();
				m_mutSendPackets.unlock();

				if (bEmpty)
					std::this_thread::sleep_for(std::chrono::milliseconds(1));
			}
		}
	}

	void TcpPool::acceptConnections()
	{
		m_mutNewSockets.lock();

		m_vecSockets.insert(m_vecSockets.end(), m_vecNewSockets.begin(), m_vecNewSockets.end());
		m_vecNewSockets.clear();

		m_mutNewSockets.unlock();
	}

	bool TcpPool::cleanupConnection(ConnectionIdentifier nId, std::shared_ptr<Socket> pSocket)
	{
		/*
			Retrieves socket status
		*/
		bool bAlive = pSocket->getAlive();

		/*
			Checks if user is manually disconnected by the removeSocket function 
		*/
		m_mutDisconnectSockets.lock();

			if (!m_vecDisconnectSockets.empty())
			{
				for (auto it = m_vecDisconnectSockets.begin(); it != m_vecDisconnectSockets.end(); ++it)
				{
					if (*it == nId)
					{
						it = m_vecDisconnectSockets.erase(it);
						bAlive = false;
						break;
					}
				}
			}

		m_mutDisconnectSockets.unlock();

		if (!bAlive)
		{
			m_mutDisconnected.lock();
			m_vecDisconnected.push_back(nId);
			m_mutDisconnected.unlock();
		}

		return !bAlive;
	}

	std::vector<TcpMessage> TcpPool::recvPackets(ConnectionIdentifier nId, std::shared_ptr<Socket> pSocket)
	{
		std::shared_ptr<Packet> pPacket = nullptr;

		/*
			Get packet from transfer list, if exists
		*/
		for (auto it = m_vecTransferPackets.begin(); it != m_vecTransferPackets.end(); ++it)
		{
			if (nId == (*it).getConnectionIdentifier())
			{
				pPacket = (*it).getPacket();
				m_vecTransferPackets.erase(it);
				break;
			}
		}

		/*
			Create new packet if required
		*/
		std::vector<TcpMessage> m_vecResult;

		auto nRecv = pSocket->read(m_pBuffer, _visNET_NETWORKBUFFER_SIZE);
		auto nLeftOver = nRecv;

		while (nLeftOver > 0)
		{
			if (!pPacket)
				pPacket = std::make_shared<Packet>();

			nLeftOver = pPacket->_onReceive(m_pBuffer + (nRecv - nLeftOver), nLeftOver);
			if (!pPacket->isValid())
				removeSocket(nId);
			else if (pPacket->isReadable())
				m_vecResult.push_back(TcpMessage(nId, pPacket));
			else if (pPacket->isTransfering())
				m_vecTransferPackets.push_back(TcpMessage(nId, pPacket));

			pPacket = nullptr;
		}

		return m_vecResult;
	}

	void TcpPool::sendPackets(ConnectionIdentifier nId, std::shared_ptr<Socket> pSocket)
	{
		m_mutSendPackets.lock();
		if (!m_vecSendPackets.empty())
		{
			for (auto it = m_vecSendPackets.begin(); it != m_vecSendPackets.end();)
			{
				if (nId == (*it).getConnectionIdentifier())
				{
					if((*it).getPacket()->_onSend())
						pSocket->write((*it).getPacket()->_getRawData(), (*it).getPacket()->_getRawSize());

					it = m_vecSendPackets.erase(it);
				}
				else
					++it;
			}
		}
		m_mutSendPackets.unlock();
	}
}
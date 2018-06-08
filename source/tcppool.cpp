#include "visnet.h"
#include "tcppool.h"

namespace visNET{
	TcpPool::TcpPool()
	{
		m_bPacketsAvailable = false;
		m_bRun = true;

		m_currentIdentifier.increase(); // Start at 1

		m_pBuffer = new uint8_t[_visNET_NETWORKBUFFER_SIZE];

		m_pThread = std::make_unique<std::thread>(&TcpPool::run, this);

		m_bKeepHistory = false;
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
		{
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
		}
		m_mutReserveID.unlock();

		return freeIdentifier;
	}

	ConnectionIdentifier TcpPool::addSocket(std::shared_ptr<Socket> s)
	{
		auto identifier = generateIdentifier();

		m_mutNewSockets.lock();
			m_vecNewSockets.push_back(IdentifiableSocket(identifier, s));
		m_mutNewSockets.unlock();

		return identifier;
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
	
	std::vector<ConnectionIdentifier> TcpPool::getDisconnected()
	{
		/* 
			Create a list of disconnected ID's
		*/

		decltype(m_vecDisconnected) retn;
		m_mutDisconnected.lock();
		{
			retn.insert(retn.end(), m_vecDisconnected.begin(), m_vecDisconnected.end());

			if (!m_bKeepHistory)
				m_vecDisconnected.clear();
		}
		m_mutDisconnected.unlock();

		// ID's are not free; Return
		if (m_bKeepHistory)
			return retn;

		/*
			Application will be notified with disconnected ID's
			Add the ID's back to the usable ID pool
		*/
		m_mutReserveID.lock();  
		{
			for (auto it = retn.begin(); it != retn.end(); ++it)
				m_queReserveID.push(*it);
		}
		m_mutReserveID.unlock();

		return retn;
	}

#ifdef _WIN32
	bool sleepForWin32(uint32_t duration)
	{
		HANDLE hTimer = CreateWaitableTimer(NULL, TRUE, NULL);
		if (hTimer == NULL)
			return false;

		LARGE_INTEGER liSleep;
		liSleep.QuadPart = -(LONGLONG)duration;

		if (!SetWaitableTimer(hTimer, &liSleep, 0, NULL, NULL, 0))
		{
			CloseHandle(hTimer);
			return false;
		}

		if (WaitForSingleObject(hTimer, INFINITE) != WAIT_OBJECT_0)
		{
			CloseHandle(hTimer);
			return false;
		}

		CloseHandle(hTimer);
		return true;
	}
#endif

	void sleepFor(uint32_t duration)
	{
#ifndef _WIN32
		std::this_thread::sleep_for(std::chrono::microseconds(duration));
#else
		if(!sleepForWin32(duration))
			std::this_thread::sleep_for(std::chrono::microseconds(duration));
#endif
	}

	void TcpPool::run()
	{
		while (m_bRun)
		{
			acceptConnections();

			std::vector<TcpMessage> receivedPackets;
			for (auto it = m_vecSockets.begin(); it != m_vecSockets.end();)
			{
				IdentifiableSocket* identifiableSocket = &(*it);

				if (cleanupConnection(identifiableSocket))
					it = m_vecSockets.erase(it);
				else
				{
					auto received = recvPackets(identifiableSocket);
					receivedPackets.insert(receivedPackets.end(), received.begin(), received.end());

					sendPackets(identifiableSocket);

					++it;
				}
			}

			/* 
				Move packets to the storage buffer
			*/
			m_mutRecvPackets.lock();
			{
				if (!receivedPackets.empty())
					m_vecRecvPackets.insert(m_vecRecvPackets.end(), receivedPackets.begin(), receivedPackets.end());

				m_bPacketsAvailable = !m_vecRecvPackets.empty();
			}
			m_mutRecvPackets.unlock();

			/*
				Pause when there are no packets to be send and none in transmission
				If the lock fails, skip this step commpletely
			*/
			if (m_mutSendPackets.try_lock())
			{
				const bool bEmpty = m_vecSendPackets.empty();
				m_mutSendPackets.unlock();

				if (bEmpty)
					sleepFor(100); // Units in microseconds
			}
		}
	}

	void TcpPool::acceptConnections()
	{
		m_mutNewSockets.lock();
		{
			m_vecSockets.insert(m_vecSockets.end(), m_vecNewSockets.begin(), m_vecNewSockets.end());
			m_vecNewSockets.clear();
		}
		m_mutNewSockets.unlock();
	}

	bool TcpPool::cleanupConnection(IdentifiableSocket* identifiableSocket)
	{
		/*
			Retrieves socket status
		*/
		bool socketAlive = identifiableSocket->getSocket()->getAlive();

		/*
			Checks if user is manually disconnected by the removeSocket function 
		*/
		m_mutDisconnectSockets.lock();
		{
			if (!m_vecDisconnectSockets.empty())
			{
				for (auto it = m_vecDisconnectSockets.begin(); it != m_vecDisconnectSockets.end(); ++it)
				{
					if ((*it) == identifiableSocket->getConnectionIdentifier())
					{
						it = m_vecDisconnectSockets.erase(it);
						socketAlive = false;
						break;
					}
				}
			}
		}
		m_mutDisconnectSockets.unlock();

		if (!socketAlive)
		{
			m_mutDisconnected.lock();
			m_vecDisconnected.push_back(identifiableSocket->getConnectionIdentifier());
			m_mutDisconnected.unlock();
		}

		return !socketAlive;
	}

	std::vector<TcpMessage> TcpPool::recvPackets(IdentifiableSocket* identifiableSocket)
	{
		std::shared_ptr<Packet> transferingPacket = nullptr;

		/*
			Get packet from transfer list, if exists
		*/
		for (auto it = m_vecTransferPackets.begin(); it != m_vecTransferPackets.end(); ++it)
		{
			if (identifiableSocket->getConnectionIdentifier() == (*it).getConnectionIdentifier())
			{
				transferingPacket = (*it).getPacket();
				m_vecTransferPackets.erase(it);
				break;
			}
		}

		/*
			Create new packet if required
		*/
		std::vector<TcpMessage> receivedMessages;

		auto receivedBytes = identifiableSocket->getSocket()->read(m_pBuffer, _visNET_NETWORKBUFFER_SIZE);
		auto remainingBytes = receivedBytes;

		while (remainingBytes > 0)
		{
			if (!transferingPacket)
				transferingPacket = std::make_shared<Packet>();

			remainingBytes = transferingPacket->_onReceive(m_pBuffer + (receivedBytes - remainingBytes), remainingBytes);
			if (!transferingPacket->isValid())
				removeSocket(identifiableSocket->getConnectionIdentifier());
			else if (transferingPacket->isReadable())
				receivedMessages.push_back(TcpMessage(identifiableSocket->getConnectionIdentifier(), transferingPacket));
			else if (transferingPacket->isTransfering())
				m_vecTransferPackets.push_back(TcpMessage(identifiableSocket->getConnectionIdentifier(), transferingPacket));

			transferingPacket = nullptr;
		}

		return receivedMessages;
	}

	void TcpPool::sendPackets(IdentifiableSocket* identifiableSocket)
	{
		m_mutSendPackets.lock();
		{
			if (!m_vecSendPackets.empty())
			{
				for (auto it = m_vecSendPackets.begin(); it != m_vecSendPackets.end();)
				{
					if (identifiableSocket->getConnectionIdentifier() == (*it).getConnectionIdentifier())
					{
						if ((*it).getPacket()->_onSend())
							identifiableSocket->getSocket()->write((*it).getPacket()->_getRawData(), (*it).getPacket()->_getRawSize());

						it = m_vecSendPackets.erase(it);
					}
					else
						++it;
				}
			}
		}
		m_mutSendPackets.unlock();
	}
}
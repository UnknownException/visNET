#include "visnet.h"
#include "socketpool.h"

namespace visNET{
	SocketPool::SocketPool()
	{
		m_bPacketsAvailable = false;
		m_bRun = true;

		m_nSocketID = 0;

		m_pThread = std::make_unique<std::thread>(&SocketPool::run, this);
	}

	SocketPool::~SocketPool()
	{
		m_bRun = false;
		m_pThread->join();
	}

	auto SocketPool::getFreeID() -> decltype(m_nSocketID)
	{ 
		decltype(m_nSocketID) id = 0;

		m_mutReserveID.lock();
		if (m_queReserveID.empty())
			id = ++m_nSocketID;
		else
		{
			id = m_queReserveID.front();
			m_queReserveID.pop();
		}
		m_mutReserveID.unlock();

		return id;
	}

	auto SocketPool::addSocket(std::unique_ptr<Socket> s) -> decltype(m_nSocketID)
	{
		if (s->getSocket() == INVALID_SOCKET)
			return 0;

		s->setNonBlocking(true);
		auto id = getFreeID();

		m_mutNewSockets.lock();
			m_vecNewSockets.push_back(std::make_pair(id, std::move(s)));
		m_mutNewSockets.unlock();

		return id;
	}

	void SocketPool::removeSocket(decltype(m_nSocketID) nSocketID)
	{
		//Disconnect
	}

	auto SocketPool::getPackets() -> decltype(m_vecRecvPackets)
	{
		decltype(m_vecRecvPackets) packets;

		if (!m_bPacketsAvailable)
			return packets;

		m_mutRecvPackets.lock();
			packets.swap(m_vecRecvPackets);
		m_mutRecvPackets.unlock();

		m_bPacketsAvailable = false;

		return packets;
	}

	void SocketPool::sendPacket(decltype(m_nSocketID) nSocketID, std::shared_ptr<RawPacket> pPacket)
	{
		m_mutSendPackets.lock();
			m_vecSendPackets.push_back(std::make_pair(nSocketID, pPacket));
		m_mutSendPackets.unlock();
	}

	void SocketPool::run()
	{
		while (m_bRun)
		{
			m_mutNewSockets.lock();
				for (auto it = m_vecNewSockets.begin(); it != m_vecNewSockets.end(); ++it)
					m_vecSockets.push_back(std::make_pair((*it).first, std::move((*it).second)));

				m_vecNewSockets.clear();
			m_mutNewSockets.unlock();

			decltype(m_vecRecvPackets) packets;
			for (auto it = m_vecSockets.begin(); it != m_vecSockets.end();)
			{
				auto packet = std::make_shared<RawPacket>();
				if ((*it).second->read(*packet))
					packets.push_back(std::make_pair((*it).first, packet));

				m_mutSendPackets.lock();
					if (!m_vecSendPackets.empty())
					{
						for (auto snd_it = m_vecSendPackets.begin(); snd_it != m_vecSendPackets.end();)
						{
							if ((*it).first == (*snd_it).first)
							{
								(*it).second->write(*(*snd_it).second);
								snd_it = m_vecSendPackets.erase(snd_it);
							}
							else
								++snd_it;
						}
					}
				m_mutSendPackets.unlock();

				if (!(*it).second->getAlive())
				{
					m_mutReserveID.lock();
						m_queReserveID.push((*it).first);
					m_mutReserveID.unlock();
					it = m_vecSockets.erase(it);
				}
				else
					++it;
			}
		
			m_mutRecvPackets.lock();
				if (!packets.empty())
					m_vecRecvPackets.insert(m_vecRecvPackets.end(), packets.begin(), packets.end());

				m_bPacketsAvailable = !m_vecRecvPackets.empty();
			m_mutRecvPackets.unlock();

			std::this_thread::sleep_for(std::chrono::milliseconds(50)); //Replace with poll
		}
	}
}
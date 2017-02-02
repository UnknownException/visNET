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
		//Should only be accessed from the ::run thread
		decltype(m_nSocketID) id = 0;

		if (m_queReserveID.empty())
			id = m_nSocketID++;
		else
		{
			id = m_queReserveID.front();
			m_queReserveID.pop();
		}

		return id;
	}

	bool SocketPool::addSocket(std::unique_ptr<Socket> s)
	{
		if (s->getSocket() == INVALID_SOCKET)
			return false;

		m_mutNewSockets.lock();
			m_vecNewSockets.push_back(std::move(s));
		m_mutNewSockets.unlock();

		return true;
	}

	auto SocketPool::getPackets() -> decltype(m_vecPackets)
	{
		decltype(m_vecPackets) packets;

		if (!m_bPacketsAvailable)
			return packets;

		m_mutPackets.lock();
			packets.swap(m_vecPackets);
		m_mutPackets.unlock();

		m_bPacketsAvailable = false;

		return packets;
	}

	void SocketPool::run()
	{
		while (m_bRun)
		{
			m_mutNewSockets.lock();
				for (auto it = m_vecNewSockets.begin(); it != m_vecNewSockets.end(); ++it)
					m_vecSockets.push_back(std::make_pair(getFreeID(), std::move(*it)));

				m_vecNewSockets.clear();
			m_mutNewSockets.unlock();

			decltype(m_vecPackets) packets;
			for (auto it = m_vecSockets.begin(); it != m_vecSockets.end();)
			{
				auto packet = std::make_shared<RawPacket>();
				if ((*it).second->read(*packet))
					packets.push_back(std::make_pair((*it).first, packet));
				
				if (!(*it).second->getAlive())
				{
					m_queReserveID.push((*it).first);
					it = m_vecSockets.erase(it);
				}
				else
					++it;
			}
		
			m_mutPackets.lock();
				m_vecPackets.insert(m_vecPackets.end(), packets.begin(), packets.end());
				m_bPacketsAvailable = !m_vecPackets.empty();
			m_mutPackets.unlock();
		}
	}
}
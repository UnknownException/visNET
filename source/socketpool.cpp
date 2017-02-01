#include "visnet.h"
#include "socketpool.h"

namespace visNET{
	SocketPool::SocketPool()
	{
		m_bPacketsAvailable = false;
		m_bRun = true;

		m_pThread = std::make_unique<std::thread>(&SocketPool::run, this);
	}

	SocketPool::~SocketPool()
	{
		m_bRun = false;
		m_pThread->join();
	}

	bool SocketPool::addSocket(std::unique_ptr<Socket> s)
	{
		if (s->getSocket() == INVALID_SOCKET)
			return false;

		m_mutNewConnection.lock();
			m_vecNewConnection.push_back(std::move(s));
		m_mutNewConnection.unlock();

		return true;
	}

	std::vector<RawPacket> SocketPool::getPackets()
	{
		std::vector<RawPacket> packets;

		if (!m_bPacketsAvailable)
			return packets;

		m_mutPacketReady.lock();
			packets.swap(m_vecPacketReady);
		m_mutPacketReady.unlock();

		return packets;
	}

	void SocketPool::run()
	{
		while (m_bRun)
		{
			m_mutNewConnection.lock();
				for (auto it = m_vecNewConnection.begin(); it != m_vecNewConnection.end(); ++it)
					m_vecConnected.push_back(std::move(*it));

				m_vecNewConnection.clear();
			m_mutNewConnection.unlock();

			std::vector<RawPacket> packets;
			for (auto it = m_vecConnected.begin(); it != m_vecConnected.end(); )
			{
				RawPacket packet;
				if ((*it)->read(packet))
					packets.push_back(packet);

				if (!(*it)->getAlive())
					it = m_vecConnected.erase(it);
				else
					++it;
			}
		
			m_mutPacketReady.lock();
				m_vecPacketReady.insert(m_vecPacketReady.end(), packets.begin(), packets.end());
			m_mutPacketReady.unlock();
		}
	}
}
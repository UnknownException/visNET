#pragma once
#include <vector>
#include <atomic>
#include <mutex>
#include <thread>
#include <queue>

namespace visNET{
	template <class PacketTemplate>
	class SocketPool{
		uint32_t m_nSocketID;
		std::mutex m_mutReserveID;
		std::queue<decltype(m_nSocketID)> m_queReserveID;

		std::mutex m_mutSendPackets;
		std::vector<std::pair<decltype(m_nSocketID), std::shared_ptr<PacketTemplate>>> m_vecSendPackets;

		std::atomic<bool> m_bPacketsAvailable;
		std::mutex m_mutRecvPackets;
		std::vector<std::pair<decltype(m_nSocketID), std::shared_ptr<PacketTemplate>>> m_vecRecvPackets;

		std::mutex m_mutNewSockets;
		std::vector<std::pair<decltype(m_nSocketID), std::unique_ptr<Socket>>> m_vecNewSockets;
		std::vector<std::pair<decltype(m_nSocketID), std::unique_ptr<Socket>>> m_vecSockets;

		std::atomic<bool> m_bRun;
		std::unique_ptr<std::thread> m_pThread;
	public:
		SocketPool();
		virtual ~SocketPool();

	private:
		auto getFreeID() -> decltype(m_nSocketID);
	public:
		auto addSocket(std::unique_ptr<Socket> s) -> decltype(m_nSocketID);
		void removeSocket(decltype(m_nSocketID));

		auto getPackets() -> decltype(m_vecRecvPackets);
		void sendPacket(decltype(m_nSocketID) nSocketID, std::shared_ptr<PacketTemplate> pPacket);
	private:
		void run();
	};

	template <class PacketTemplate>
	SocketPool<PacketTemplate>::SocketPool()
	{
		m_bPacketsAvailable = false;
		m_bRun = true;

		m_nSocketID = 0;

		m_pThread = std::make_unique<std::thread>(&SocketPool::run, this);
	}

	template <class PacketTemplate>
	SocketPool<PacketTemplate>::~SocketPool()
	{
		m_bRun = false;
		m_pThread->join();
	}

	template <class PacketTemplate>
	auto SocketPool<PacketTemplate>::getFreeID() -> decltype(m_nSocketID)
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

	template <class PacketTemplate>
	auto SocketPool<PacketTemplate>::addSocket(std::unique_ptr<Socket> s) -> decltype(m_nSocketID)
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

	template <class PacketTemplate>
	void SocketPool<PacketTemplate>::removeSocket(decltype(m_nSocketID) nSocketID)
	{
		//Disconnect
	}

	template <class PacketTemplate>
	auto SocketPool<PacketTemplate>::getPackets() -> decltype(m_vecRecvPackets)
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

	template <class PacketTemplate>
	void SocketPool<PacketTemplate>::sendPacket(decltype(m_nSocketID) nSocketID, std::shared_ptr<PacketTemplate> pPacket)
	{
		m_mutSendPackets.lock();
		m_vecSendPackets.push_back(std::make_pair(nSocketID, pPacket));
		m_mutSendPackets.unlock();
	}

	template <class PacketTemplate>
	void SocketPool<PacketTemplate>::run()
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
				auto packet = std::make_shared<PacketTemplate>();
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

			if(m_mutSendPackets.try_lock())
			{ 
				// No packets available to send
				bool bEmpty = m_vecSendPackets.empty();
				m_mutSendPackets.unlock(); // Don't keep lock

				if(!bEmpty)
					std::this_thread::sleep_for(std::chrono::milliseconds(1));
			}
		}
	}
}
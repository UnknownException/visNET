#pragma once
#include <vector>
#include <atomic>
#include <mutex>
#include <thread>
#include <algorithm>
#include <queue>

namespace visNET{
	class SocketPool{
		uint32_t m_nSocketID;
		std::queue<decltype(m_nSocketID)> m_queReserveID;

		std::atomic<bool> m_bPacketsAvailable;
		std::mutex m_mutPackets;
		std::vector<std::pair<decltype(m_nSocketID), std::shared_ptr<RawPacket>>> m_vecPackets;

		std::mutex m_mutNewSockets;
		std::vector<std::unique_ptr<Socket>> m_vecNewSockets;
		std::vector<std::pair<decltype(m_nSocketID), std::unique_ptr<Socket>>> m_vecSockets;

		std::atomic<bool> m_bRun;
		std::unique_ptr<std::thread> m_pThread;
	public:
		SocketPool();
		virtual ~SocketPool();

	private:
		auto getFreeID() -> decltype(m_nSocketID);
	public:
		bool addSocket(std::unique_ptr<Socket> s);
		auto getPackets() -> decltype(m_vecPackets);

	private:
		void run();
	};
}
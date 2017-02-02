#pragma once
#include <vector>
#include <atomic>
#include <mutex>
#include <thread>
#include <queue>

namespace visNET{
	class SocketPool{
		uint32_t m_nSocketID;
		std::mutex m_mutReserveID;
		std::queue<decltype(m_nSocketID)> m_queReserveID;

		std::mutex m_mutSendPackets;
		std::vector<std::pair<decltype(m_nSocketID), std::shared_ptr<RawPacket>>> m_vecSendPackets;

		std::atomic<bool> m_bPacketsAvailable;
		std::mutex m_mutRecvPackets;
		std::vector<std::pair<decltype(m_nSocketID), std::shared_ptr<RawPacket>>> m_vecRecvPackets;

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
		void sendPacket(decltype(m_nSocketID) nSocketID, std::shared_ptr<RawPacket> pPacket);
	private:
		void run();
	};
}
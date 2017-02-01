#pragma once
#include <vector>
#include <atomic>
#include <mutex>
#include <thread>
#include <algorithm>

namespace visNET{
	class SocketPool{
		std::atomic<bool> m_bPacketsAvailable;

		std::mutex m_mutPacketReady;
		std::vector<RawPacket> m_vecPacketReady;

		std::mutex m_mutNewConnection;
		std::vector<std::unique_ptr<Socket>> m_vecNewConnection;

		std::vector<std::unique_ptr<Socket>> m_vecConnected;

		std::atomic<bool> m_bRun;
		std::unique_ptr<std::thread> m_pThread;
	public:
		SocketPool();
		virtual ~SocketPool();

		bool addSocket(std::unique_ptr<Socket> s);
		std::vector<RawPacket> getPackets();

	private:
		void run();
	};
}
#pragma once

/*
	Manages TCP input and output stream
	Converts reveived chunks back into complete packets
*/

namespace visNETCore{
	class TcpPool{
		uint32_t m_nSocketID;
		std::mutex m_mutReserveID;
		std::queue<decltype(m_nSocketID)> m_queReserveID;

		std::mutex m_mutSendPackets;
		std::vector<std::pair<decltype(m_nSocketID), std::shared_ptr<Packet>>> m_vecSendPackets;

		std::atomic<bool> m_bPacketsAvailable;
		std::mutex m_mutRecvPackets;
		std::vector<std::pair<decltype(m_nSocketID), std::shared_ptr<Packet>>> m_vecRecvPackets;

		std::mutex m_mutNewSockets;
		std::vector<std::pair<decltype(m_nSocketID), std::shared_ptr<Socket>>> m_vecNewSockets;
		std::mutex m_mutDisconnectSockets;
		std::vector<decltype(m_nSocketID)> m_vecDisconnectSockets;
		std::vector<std::pair<decltype(m_nSocketID), std::shared_ptr<Socket>>> m_vecSockets;

		std::mutex m_mutDisconnected;
		std::vector<decltype(m_nSocketID)> m_vecDisconnected;

		std::atomic<bool> m_bRun;
		std::unique_ptr<std::thread> m_pThread;

		uint8_t* m_pBuffer;
	public:
		TcpPool();
		virtual ~TcpPool();

	private:
		auto getFreeID() -> decltype(m_nSocketID);
	public:
		auto addSocket(std::shared_ptr<Socket> s) -> decltype(m_nSocketID);
		void removeSocket(decltype(m_nSocketID));

		auto getPackets() -> decltype(m_vecRecvPackets);
		void sendPacket(decltype(m_nSocketID) nSocketID, std::shared_ptr<Packet> pPacket);
		std::vector<decltype(m_nSocketID)> getDisconnected(bool bKeepHistory);
	private:
		void run();
		void acceptConnections();
		bool cleanupConnection(decltype(m_nSocketID), std::shared_ptr<Socket> pSocket);

		std::vector<std::pair<decltype(m_nSocketID), std::shared_ptr<Packet>>> m_vecTransferPackets;
		std::vector<std::pair<decltype(m_nSocketID), std::shared_ptr<Packet>>> recvPackets(decltype(m_nSocketID), std::shared_ptr<Socket> pSocket);
		void sendPackets(decltype(m_nSocketID), std::shared_ptr<Socket> pSocket);
	};
}
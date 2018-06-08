#pragma once

#include <thread>
#include <queue>
#include <atomic>
#include <mutex>

/*
	Manages TCP input and output stream
	Converts reveived chunks back into complete packets
*/

namespace visNETCore{
	class TcpPool{
		ConnectionIdentifier m_currentIdentifier;
		std::mutex m_mutReserveID;
		std::queue<ConnectionIdentifier> m_queReserveID;

		std::mutex m_mutSendPackets;
		std::vector<TcpMessage> m_vecSendPackets;

		std::atomic<bool> m_bPacketsAvailable;
		std::mutex m_mutRecvPackets;
		std::vector<TcpMessage> m_vecRecvPackets;

		std::mutex m_mutNewSockets;
		std::vector<IdentifiableSocket> m_vecNewSockets;
		std::mutex m_mutDisconnectSockets;
		std::vector<ConnectionIdentifier> m_vecDisconnectSockets;
		std::vector<IdentifiableSocket> m_vecSockets;

		std::mutex m_mutDisconnected;
		std::vector<ConnectionIdentifier> m_vecDisconnected;

		std::atomic<bool> m_bRun;
		std::unique_ptr<std::thread> m_pThread;

		uint8_t* m_pBuffer;

		bool m_bKeepHistory;
	public:
		TcpPool();
		virtual ~TcpPool();

	private:
		ConnectionIdentifier generateIdentifier();
	public:
		ConnectionIdentifier addSocket(std::shared_ptr<Socket> s);
		void removeSocket(ConnectionIdentifier id);
		std::vector<ConnectionIdentifier> getDisconnected();
		void SetKeepHistory(bool b) { m_bKeepHistory = b; }

		std::vector<TcpMessage> getPackets();
		void sendPacket(TcpMessage& message);
	private:
		// Network thread variables and functionality
		std::vector<TcpMessage> m_vecTransferPackets; 

		void run();

		void acceptConnections();
		bool cleanupConnection(IdentifiableSocket* identifiableSocket);
		std::vector<TcpMessage> recvPackets(IdentifiableSocket* identifiableSocket);
		void sendPackets(IdentifiableSocket* identifiableSocket);
	};
}
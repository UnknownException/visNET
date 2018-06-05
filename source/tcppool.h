#pragma once

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
		std::vector<std::pair<ConnectionIdentifier, std::shared_ptr<Socket>>> m_vecNewSockets;
		std::mutex m_mutDisconnectSockets;
		std::vector<ConnectionIdentifier> m_vecDisconnectSockets;
		std::vector<std::pair<ConnectionIdentifier, std::shared_ptr<Socket>>> m_vecSockets;

		std::mutex m_mutDisconnected;
		std::vector<ConnectionIdentifier> m_vecDisconnected;

		std::atomic<bool> m_bRun;
		std::unique_ptr<std::thread> m_pThread;

		uint8_t* m_pBuffer;
	public:
		TcpPool();
		virtual ~TcpPool();

	private:
		ConnectionIdentifier generateIdentifier();
	public:
		ConnectionIdentifier addSocket(std::shared_ptr<Socket> s);
		void removeSocket(ConnectionIdentifier id);
		std::vector<ConnectionIdentifier> getDisconnected(bool bKeepHistory);

		std::vector<TcpMessage> getPackets();
		void sendPacket(TcpMessage& message);
	private:
		void run();
		void acceptConnections();
		bool cleanupConnection(ConnectionIdentifier id, std::shared_ptr<Socket> pSocket);

		std::vector<TcpMessage> m_vecTransferPackets;
		std::vector<TcpMessage> recvPackets(ConnectionIdentifier id, std::shared_ptr<Socket> pSocket);
		void sendPackets(ConnectionIdentifier id, std::shared_ptr<Socket> pSocket);
	};
}
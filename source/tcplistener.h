#pragma once

namespace visNET{
	class DLLEXPORT TcpListener {
		TcpSocket* m_pSocket;
		TcpPool* m_pTcpPool;
		bool m_bValid;
	public:
		TcpListener(uint16_t nPort);
		virtual ~TcpListener();

		ConnectionIdentifier getConnection();

		void send(TcpMessage& message);
		void send(ConnectionIdentifier identifier, Packet& packet);

		std::vector<TcpMessage> receive();
		std::vector<ConnectionIdentifier> getDisconnected();
		void disconnect(ConnectionIdentifier nClId);
		bool isValid() { return m_bValid; }
	};
}
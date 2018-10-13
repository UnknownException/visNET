#pragma once

/*
	Client is the client sided connection
	This class will connect to a listener
*/

namespace visNET{
	class __declspec(dllexport) TcpClient {
		TcpPool* m_pTcpPool;
		ConnectionIdentifier m_serverIdentifier;
		bool m_bValid;
	public:
		TcpClient(const char* pszIp, uint16_t nPort);
		virtual ~TcpClient();

		void send(std::shared_ptr<Packet> pPacket);
		void send(Packet& pPacket);
		std::vector<TcpMessage> receive();
		bool isDisconnected();
		bool isValid() { return m_bValid; }
	};
}
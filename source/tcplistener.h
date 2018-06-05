#pragma once
#include "networkobject.h"

namespace visNETCore{
	class TcpListener : public NetworkObject{
		TcpPool* m_pTcpPool;

	public:
		TcpListener(uint16_t nPort);
		virtual ~TcpListener();

		ConnectionIdentifier getConnection();

		void send(TcpMessage& message);
		std::vector<TcpMessage> getPackets();
		std::vector<ConnectionIdentifier> getDisconnected();
		void disconnect(ConnectionIdentifier nClId);
	};
}
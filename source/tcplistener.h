#pragma once
#include "networkobject.h"

namespace visNET{
	class __declspec(dllexport) TcpListener : public NetworkObject{
#ifdef _BUILDLIBRARY
		TcpPool* m_pTcpPool;
#else
		void* m_pTcpPool;
#endif
	public:
		TcpListener(uint16_t nPort);
		virtual ~TcpListener();

		ConnectionIdentifier getConnection();

		void send(TcpMessage& message);
		void send(ConnectionIdentifier identifier, Packet& packet);

		std::vector<TcpMessage> receive();
		std::vector<ConnectionIdentifier> getDisconnected();
		void disconnect(ConnectionIdentifier nClId);
	};
}
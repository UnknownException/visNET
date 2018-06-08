#pragma once
#include <stdint.h>
#include <memory>
#include <vector>
#include "../../source/blobarray.h"
#include "../../source/connectionidentifier.h"
#include "../../source/packet.h"
#include "../../source/tcpmessage.h"
#include "../../source/udpmessage.h"

namespace visNET {
	/* Base Interface */
	__declspec(dllexport) bool startup();
	__declspec(dllexport) bool cleanup();

	/* Tcp Listener Interface */
	class __declspec(dllexport) TcpListener {
		void* tcpListener;
	public:
		TcpListener(uint16_t nPort);
		virtual ~TcpListener();

		ConnectionIdentifier getConnection();

		void send(TcpMessage& message);
		void send(ConnectionIdentifier identifier, Packet& packet);
		std::vector<TcpMessage> getPackets();
		std::vector<ConnectionIdentifier> getDisconnected();
		void disconnect(ConnectionIdentifier identifier);

		bool isValid();
		std::string getError();
	};

	/* Tcp Client Interface */
	class __declspec(dllexport) TcpClient{
		void* tcpClient;

	public:
		TcpClient(const char* pszIp, uint16_t nPort);
		virtual ~TcpClient();

		void send(Packet& pPacket);
		std::vector<TcpMessage> getPackets();
		bool isDisconnected();

		bool isValid();
		std::string getError();
	};

	/* Udp Client Interface */
	class __declspec(dllexport) UdpClient {
		void* udpClient;

	public:
		UdpClient(uint16_t nPort);
		virtual ~UdpClient();

		bool send(UdpMessage& message);
		bool send(std::string ip, uint16_t port, Packet& packet);
		std::vector<UdpMessage> getPackets();

		bool isValid();
		std::string getError();
	};
}
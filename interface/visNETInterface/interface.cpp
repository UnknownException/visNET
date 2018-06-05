#include "include.h"
#include "../../source/visnet.h"

namespace visNET {
	bool startup()
	{
		return visNETCore::startup();
	}

	bool cleanup()
	{
		return visNETCore::cleanup();
	}

	/*
		TCP Listener Interface
	*/

	TcpListener::TcpListener(uint16_t nPort) 
	{
		tcpListener = new visNETCore::TcpListener(nPort);
	}
	
	TcpListener::~TcpListener()
	{
		delete tcpListener;
	}

	ConnectionIdentifier TcpListener::getConnection()
	{
		return ((visNETCore::TcpListener*)tcpListener)->getConnection();
	}

	void TcpListener::send(TcpMessage& message)
	{
		((visNETCore::TcpListener*)tcpListener)->send(message);
	}

	void TcpListener::send(ConnectionIdentifier identifier, Packet& packet)
	{
		send(TcpMessage(identifier, packet._copy()));
	}

	std::vector<TcpMessage> TcpListener::getPackets()
	{
		return ((visNETCore::TcpListener*)tcpListener)->getPackets();
	}

	std::vector<ConnectionIdentifier> TcpListener::getDisconnected()
	{
		return ((visNETCore::TcpListener*)tcpListener)->getDisconnected();
	}

	void TcpListener::disconnect(ConnectionIdentifier identifier)
	{
		return ((visNETCore::TcpListener*)tcpListener)->disconnect(identifier);
	}

	bool TcpListener::isValid()
	{
		return ((visNETCore::TcpListener*)tcpListener)->isValid();
	}

	std::string TcpListener::getError()
	{
		return  ((visNETCore::TcpListener*)tcpListener)->getError();
	}


	/*
		TCP Client Interface
	*/

	TcpClient::TcpClient(const char* pszIp, uint16_t nPort)
	{
		tcpClient = new visNETCore::TcpClient(pszIp, nPort);
	}

	TcpClient::~TcpClient()
	{
		delete tcpClient;
	}

	void TcpClient::send(Packet& packet)
	{
		((visNETCore::TcpClient*)tcpClient)->send(packet._copy());
	}

	std::vector<TcpMessage> TcpClient::getPackets()
	{
		return ((visNETCore::TcpClient*)tcpClient)->getPackets();
	}

	bool TcpClient::isDisconnected()
	{
		return ((visNETCore::TcpClient*)tcpClient)->isDisconnected();
	}

	bool TcpClient::isValid()
	{
		return ((visNETCore::TcpClient*)tcpClient)->isValid();
	}

	std::string TcpClient::getError()
	{
		return  ((visNETCore::TcpClient*)tcpClient)->getError();
	}

	/* 
		UDP Client Interface
	*/

	UdpClient::UdpClient(uint16_t nPort)
	{
		udpClient = new visNETCore::UdpClient(nPort);
	}

	UdpClient::~UdpClient()
	{
		delete udpClient;
	}

	bool UdpClient::send(UdpMessage& message)
	{
		return ((visNETCore::UdpClient*)udpClient)->send(message);
	}

	bool UdpClient::send(std::string ip, uint16_t port, Packet& packet)
	{
		return send(UdpMessage(ip, port, packet._copy()));
	}

	std::vector<UdpMessage> UdpClient::getPackets()
	{
		return ((visNETCore::UdpClient*)udpClient)->getPackets();
	}

	bool UdpClient::isValid()
	{
		return ((visNETCore::UdpClient*)udpClient)->isValid();
	}

	std::string UdpClient::getError()
	{
		return  ((visNETCore::UdpClient*)udpClient)->getError();
	}
}
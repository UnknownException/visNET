#include "pch.h"

TEST(TcpTest, ConstructConnection)
{
	/* Create TCP listener and client */
	EXPECT_TRUE(visNET::startup()) << "Failed to startup";

	visNET::TcpListener tcpSvr(TESTTCP_PORT_CONSTRUCTCONNECTION1);
	EXPECT_TRUE(tcpSvr.isValid()) << "Failed to initialize TcpListener";

	visNET::TcpClient tcpCl("127.0.0.1", TESTTCP_PORT_CONSTRUCTCONNECTION1);
	EXPECT_TRUE(tcpCl.isValid()) << "Failed to initialize TcpClient";

	/* Try to create second listener on the same port */
	visNET::TcpListener tcpSvrBad(TESTTCP_PORT_CONSTRUCTCONNECTION1);
	EXPECT_FALSE(tcpSvrBad.isValid()) << "Managed to create second listener on same port";

	/* Try to create a client that connects to a non existing listener */
	visNET::TcpClient tcpClBad("127.0.0.1", TESTTCP_PORT_CONSTRUCTCONNECTION2);
	EXPECT_FALSE(tcpClBad.isValid()) << "Managed to connect to an unknown listener";

	/* Try to create second listener on a different port */
	visNET::TcpListener tcpSvrSecond(TESTTCP_PORT_CONSTRUCTCONNECTION2);
	EXPECT_TRUE(tcpSvrSecond.isValid()) << "Couldn't create second listener";

	/* Try to connect to second listener */
	visNET::TcpClient tcpClSecond("127.0.0.1", TESTTCP_PORT_CONSTRUCTCONNECTION1);
	EXPECT_TRUE(tcpClSecond.isValid()) << "Failed to connect to second listener";

	/* Shutdown Winsock */
	EXPECT_TRUE(visNET::cleanup()) << "Failed to cleanup";
}

TEST(TcpTest, DisconnectClient)
{
	/* Create TCP listener and client */
	EXPECT_TRUE(visNET::startup()) << "Failed to startup";

	visNET::TcpListener tcpSvr(TESTTCP_PORT8);
	EXPECT_TRUE(tcpSvr.isValid()) << "Failed to initialize TcpListener";

	visNET::TcpClient tcpCl("127.0.0.1", TESTTCP_PORT8);
	EXPECT_TRUE(tcpCl.isValid()) << "Failed to initialize TcpClient";

	int32_t nAttempts = 50;

	/* Accept Connection */
	auto connectionId = tcpSvr.getConnection();
	while (connectionId == 0)
	{
		EXPECT_NE(0, nAttempts--) << "Failed to establish a connection";

		// Halt execution
		std::this_thread::sleep_for(std::chrono::microseconds(1));

		connectionId = tcpSvr.getConnection();
	}

	tcpSvr.disconnect(connectionId);

	auto disconnected = tcpSvr.getDisconnected();
	while (disconnected.empty())
	{
		EXPECT_NE(0, nAttempts--) << "Failed to disconnect the connection";

		// Halt execution
		std::this_thread::sleep_for(std::chrono::microseconds(1));

		disconnected = tcpSvr.getDisconnected();
	}

	if (connectionId != disconnected.at(0))
		EXPECT_TRUE(false) << "Disconnection id does not match";

	visNET::Packet packet;
	tcpCl.send(packet);

	std::this_thread::sleep_for(std::chrono::microseconds(1000));

	EXPECT_TRUE(tcpCl.isDisconnected()) << "TCP client is giving the wrong state";

	/* Shutdown Winsock */
	EXPECT_TRUE(visNET::cleanup()) << "Failed to cleanup";
}


TEST(TcpTest, SendBigPacketsToClient)
{
	/* Create TCP listener and client */
	EXPECT_TRUE(visNET::startup()) << "Failed to startup";

	visNET::TcpListener tcpSvr(TESTTCP_PORT6);
	EXPECT_TRUE(tcpSvr.isValid()) << "Failed to initialize TcpListener";

	visNET::TcpClient tcpCl("127.0.0.1", TESTTCP_PORT6);
	EXPECT_TRUE(tcpCl.isValid()) << "Failed to initialize TcpClient";

	/* Receive attempts */
	int32_t nAttempts = 50;

	/* Accept Connection */
	auto connectionId = tcpSvr.getConnection();
	while (connectionId == 0)
	{
		EXPECT_NE(0, nAttempts--) << "Failed to establish a connection";

		// Halt execution
		std::this_thread::sleep_for(std::chrono::microseconds(1));

		connectionId = tcpSvr.getConnection();
	}

	/* Reset attempts */
	nAttempts = 50;

	const uint32_t packetSizeLimit = 0xFFFF - 4; // 4 bytes reserved for packetsize
	const uint32_t packetsToSend = 8;

	/* Create and send test packet */
	for (uint32_t i = 0; i < packetsToSend; ++i)
	{
		visNET::Packet packet;
		for (uint32_t x = 0; x < packetSizeLimit; ++x)
			packet.writeUInt8(x % 256);

		tcpSvr.send(connectionId, packet);
	}

	/* Try to receive all send packets */
	std::vector<std::vector<uint8_t>> received;
	while (received.size() < packetsToSend)
	{
		auto recv = tcpCl.receive();
		while (recv.empty())
		{
			EXPECT_NE(0, nAttempts--) << "Failed to receive a data packet";

			// Halt execution
			std::this_thread::sleep_for(std::chrono::microseconds(1));

			recv = tcpCl.receive();
		}

		for (auto it = recv.begin(); it != recv.end(); ++it)
		{
			std::vector<uint8_t> params;
			for (uint32_t x = 0; x < packetSizeLimit; ++x)
				params.push_back((*it).getPacket()->readUInt8());

			received.push_back(params);
			EXPECT_EQ((size_t)packetSizeLimit, params.size()) << "Received different amount of parameters than expected";
		}

		/* Reset attempts */
		nAttempts = 50;
	}

	/* Check received values */
	for (uint32_t i = 0; i < packetsToSend; ++i)
	{
		for (uint32_t x = 0; x < packetSizeLimit; ++x)
			EXPECT_EQ(static_cast<uint8_t>(x % 256), received[i][x]) << "Received value is different from expected value";
	}

	/* Shutdown Winsock */
	EXPECT_TRUE(visNET::cleanup()) << "Failed to cleanup";
}

TEST(TcpTest, SendBigPacketsToServer)
{
	/* Create TCP listener and client */
	EXPECT_TRUE(visNET::startup()) << "Failed to startup";

	visNET::TcpListener tcpSvr(TESTTCP_PORT5);
	EXPECT_TRUE(tcpSvr.isValid()) << "Failed to initialize TcpListener";

	visNET::TcpClient tcpCl("127.0.0.1", TESTTCP_PORT5);
	EXPECT_TRUE(tcpCl.isValid()) << "Failed to initialize TcpClient";

	/* Receive attempts */
	int32_t nAttempts = 50;

	/* Accept Connection */
	auto connectionId = tcpSvr.getConnection();
	while (connectionId == 0)
	{
		EXPECT_NE(0, nAttempts--) << "Failed to establish a connection";

		// Halt execution
		std::this_thread::sleep_for(std::chrono::microseconds(1));

		connectionId = tcpSvr.getConnection();
	}

	/* Reset attempts */
	nAttempts = 50;

	const uint32_t packetSizeLimit = 0xFFFF - 4; // 4 bytes reserved for packetsize
	const uint32_t packetsToSend = 8;

	/* Create and send test packet */
	for (uint32_t i = 0; i < packetsToSend; ++i)
	{
		visNET::Packet packet;
		for (uint32_t x = 0; x < packetSizeLimit; ++x)
			packet.writeUInt8(x % 256);

		tcpCl.send(packet);
	}

	/* Try to receive all send packets */
	std::vector<std::vector<uint8_t>> received;
	while (received.size() < packetsToSend)
	{
		auto recv = tcpSvr.receive();
		while (recv.empty())
		{
			EXPECT_NE(0, nAttempts--) << "Failed to receive a data packet";

			// Halt execution
			std::this_thread::sleep_for(std::chrono::microseconds(1));

			recv = tcpSvr.receive();
		}

		for (auto it = recv.begin(); it != recv.end(); ++it)
		{
			std::vector<uint8_t> params;
			for (uint32_t x = 0; x < packetSizeLimit; ++x)
				params.push_back((*it).getPacket()->readUInt8());

			received.push_back(params);
			EXPECT_EQ((size_t)packetSizeLimit, params.size()) << "Received different amount of parameters than expected";
		}

		/* Reset attempts */
		nAttempts = 50;
	}

	/* Check received values */
	for (uint32_t i = 0; i < packetsToSend; ++i)
	{
		for (uint32_t x = 0; x < packetSizeLimit; ++x)
			EXPECT_EQ(static_cast<uint8_t>(x % 256), received[i][x]) << "Received value is different from expected value";
	}

	/* Shutdown Winsock */
	EXPECT_TRUE(visNET::cleanup()) << "Failed to cleanup";
}

TEST(TcpTest, SendLotsOfPacketsToClient)
{
	/* Create TCP listener and client */
	EXPECT_TRUE(visNET::startup()) << "Failed to startup";

	visNET::TcpListener tcpSvr(TESTTCP_PORT4);
	EXPECT_TRUE(tcpSvr.isValid()) << "Failed to initialize TcpListener";

	visNET::TcpClient tcpCl("127.0.0.1", TESTTCP_PORT4);
	EXPECT_TRUE(tcpCl.isValid()) << "Failed to initialize TcpClient";

	/* Receive attempts */
	int32_t nAttempts = 50;

	/* Accept Connection */
	auto connectionId = tcpSvr.getConnection();
	while (connectionId == 0)
	{
		EXPECT_NE(0, nAttempts--) << "Failed to establish a connection";

		// Halt execution
		std::this_thread::sleep_for(std::chrono::microseconds(1));

		connectionId = tcpSvr.getConnection();
	}

	/* Create and send test packet */
	for (uint32_t i = 0; i < 4096 * 8; ++i)
	{
		visNET::Packet packet;
		packet.writeUInt32(i % 844);

		tcpSvr.send(connectionId, packet);
	}

	/* Reset attempts */
	nAttempts = 50;

	/* Try to receive all send packets */
	std::vector<uint32_t> received;
	while (received.size() < 4096 * 8)
	{
		auto recv = tcpCl.receive();
		while (recv.empty())
		{
			EXPECT_NE(0, nAttempts--) << "Failed to receive a data packet";

			// Halt execution
			std::this_thread::sleep_for(std::chrono::microseconds(1));

			recv = tcpCl.receive();
		}

		for (auto it = recv.begin(); it != recv.end(); ++it)
			received.push_back((*it).getPacket()->readUInt32());

		/* Reset attempts */
		nAttempts = 50;
	}

	/* Check received values */
	for (uint32_t i = 0; i < 4096 * 8; ++i)
		EXPECT_EQ(i % 844, received[i]) << "Received value is different from expected value";

	/* Shutdown Winsock */
	EXPECT_TRUE(visNET::cleanup()) << "Failed to cleanup";
}

TEST(TcpTest, SendLotsOfPacketsToServer)
{
	/* Create TCP listener and client */
	EXPECT_TRUE(visNET::startup()) << "Failed to startup";

	visNET::TcpListener tcpSvr(TESTTCP_PORT3);
	EXPECT_TRUE(tcpSvr.isValid()) << "Failed to initialize TcpListener";

	visNET::TcpClient tcpCl("127.0.0.1", TESTTCP_PORT3);
	EXPECT_TRUE(tcpCl.isValid()) << "Failed to initialize TcpClient";

	/* Create and send test packet */
	for (uint32_t i = 0; i < 4096 * 8; ++i)
	{
		visNET::Packet packet;
		packet.writeUInt32(i % 913);

		tcpCl.send(packet);
	}

	/* Receive attempts */
	int32_t nAttempts = 50;

	/* Accept Connection */
	auto connectionId = tcpSvr.getConnection();
	while (connectionId == 0)
	{
		EXPECT_NE(0, nAttempts--) << "Failed to establish a connection";

		// Halt execution
		std::this_thread::sleep_for(std::chrono::microseconds(1));

		connectionId = tcpSvr.getConnection();
	}

	/* Reset attempts */
	nAttempts = 50;

	/* Try to receive all send packets */
	std::vector<uint32_t> received;
	while (received.size() < 4096 * 8)
	{
		auto recv = tcpSvr.receive();
		while (recv.empty())
		{
			EXPECT_NE(0, nAttempts--) << "Failed to receive a data packet";

			// Halt execution
			std::this_thread::sleep_for(std::chrono::microseconds(1));

			recv = tcpSvr.receive();
		}

		for (auto it = recv.begin(); it != recv.end(); ++it)
			received.push_back((*it).getPacket()->readUInt32());

		/* Reset attempts */
		nAttempts = 50;
	}

	/* Check received values */
	for (uint32_t i = 0; i < 4096 * 8; ++i)
		EXPECT_EQ(i % 913, received[i]) << "Received value is different from expected value";

	/* Shutdown Winsock */
	EXPECT_TRUE(visNET::cleanup()) << "Failed to cleanup";
}

uint32_t timeGetTime()
{
	EXPECT_FALSE(true) << "Time function not implemented";
	return 0;
}

TEST(TcpTest, TestTcpPerformance)
{
	uint32_t curTime = timeGetTime();

	/* Create TCP listener and client */
	EXPECT_TRUE(visNET::startup()) << "Failed to startup";

	visNET::TcpListener tcpSvr(TESTTCP_PORT7);
	EXPECT_TRUE(tcpSvr.isValid()) << "Failed to initialize TcpListener";

	visNET::TcpClient tcpCl("127.0.0.1", TESTTCP_PORT7);
	EXPECT_TRUE(tcpCl.isValid()) << "Failed to initialize TcpClient";

	/* Create Test Packet */
	visNET::Packet packet;
	packet.writeDouble(31.5f);

	/* Send Test Packet */
	tcpCl.send(packet);

	/* Receive attempts */
	int32_t nAttempts = 5000;

	/* Accept Connection */
	auto connectionId = tcpSvr.getConnection();
	while (connectionId == 0)
	{
		EXPECT_NE(0, nAttempts--) << "Failed to establish a connection";
		connectionId = tcpSvr.getConnection();
	}

	/* Reset attempts */
	nAttempts = 5000;

	/* Receive Packet */
	auto recv = tcpSvr.receive();
	while (recv.empty())
	{
		EXPECT_NE(0, nAttempts--) << "Failed to receive a data packet";
		recv = tcpSvr.receive();
	}

	auto pPacket = recv.at(0).getPacket();
	EXPECT_TRUE(pPacket->isReadable());

	EXPECT_EQ(static_cast<double>(31.5f), pPacket->readDouble()) << "Failed to read double from packet";

	// Sleep performance....
	EXPECT_FALSE(curTime + 5 < timeGetTime()) << "Failed to deliver local packet in time";

	/* Shutdown Winsock */
	EXPECT_TRUE(visNET::cleanup()) << "Failed to cleanup";
}
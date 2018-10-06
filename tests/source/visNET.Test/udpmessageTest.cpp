#include "pch.h"

TEST(Structure_UdpMessage, CreateUdpMessage)
{
	// Assign
	std::string ip = "127.0.0.1";
	uint32_t port = 1234;

	// Act
	visNET::UdpMessage message(ip, port, nullptr);

	// Assert
	EXPECT_EQ("127.0.0.1", message.getIP());
	EXPECT_EQ(1234, message.getPort());
	EXPECT_EQ(nullptr, message.getPacket());
}
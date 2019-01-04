#include "pch.h"

TEST(Structure_TcpMessage, CreateTcpMessage)
{
	// Assign
	visNET::ConnectionIdentifier id(123);

	// Act
	visNET::TcpMessage message(id, nullptr);

	// Assert
	EXPECT_EQ(123, message.getConnectionIdentifier().getValue());
	EXPECT_EQ(nullptr, message.getPacket());
}
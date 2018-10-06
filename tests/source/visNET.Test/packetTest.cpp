#include "pch.h"
#include "packetbuilder.h"

TEST(Packet_Packet, CreatePacket)
{
	// Assign
	// Act
	visNET::Packet packet;

	// Assert
	EXPECT_TRUE(packet.isWritable());
	EXPECT_TRUE(packet.isValid());
}

TEST(Packet_Packet, EmptyReceivedPacketShouldBeValid)
{
	// Assign
	// Act
	auto packet = PacketBuilder()
					.build();

	// Assert
	EXPECT_TRUE(packet->isReadable());
	EXPECT_TRUE(packet->isValid());
}

TEST(Packet_Packet, WriteToPacket)
{
	// Assign
	visNET::Packet packet;

	// Act
	packet.writeInt32(123);

	// Assert
	EXPECT_TRUE(packet.isWritable());
	EXPECT_TRUE(packet.isValid());
}


TEST(Packet_Packet, ReadFromPacket)
{
	constexpr int32_t testValue = 1234;

	// Assign
	auto packet = PacketBuilder()
					.writeInt32(testValue)
					.build();

	// Act
	int32_t result = packet->readInt32();

	// Assert
	EXPECT_EQ(testValue, result);
	EXPECT_TRUE(packet->isReadable());
	EXPECT_TRUE(packet->isValid());
}

TEST(Packet_Packet, ShouldNotReadOutsideOfPacketBounds)
{
	// Assign
	auto packet = PacketBuilder()
					.writeInt32(123)
					.build();

	// Act
	int64_t result = packet->readInt64();

	// Assert
	EXPECT_EQ(0, result);
	EXPECT_FALSE(packet->isReadable());
	EXPECT_FALSE(packet->isValid());
}

TEST(Packet_Packet, WriteMaxLength)
{
	// Assign
	visNET::Packet packet;

	// Act
	// Substract 4 because the first 4 bytes are reserved for the packet size
	for (uint32_t i = 0; i < _visNET_PACKETSIZE_LIMIT - 4; ++i)
	{
		packet.writeInt8(0);
	}

	// Assert
	EXPECT_TRUE(packet.isWritable());
	EXPECT_TRUE(packet.isValid());
}


TEST(Packet_Packet, ShouldNotWriteMoreThanMaxLength)
{
	// Assign
	visNET::Packet packet;

	// Act
	for (uint32_t i = 0; i < _visNET_PACKETSIZE_LIMIT - 3; ++i)
	{
		packet.writeInt8(0);
	}

	// Assert
	EXPECT_FALSE(packet.isWritable());
	EXPECT_FALSE(packet.isValid());
}
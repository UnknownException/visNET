#include "pch.h"

TEST(PacketTest, ConstructPacket)
{
	visNET::Packet packet;
	EXPECT_TRUE(packet.isWritable());

	packet.writeBool(true);
	packet.writeInt8(1);
	packet.writeDouble(1.f);
	packet.writeFloat(1.f);
	packet.writeInt32(1);
	packet.writeInt16(1);
	packet.writeString("Test");
	packet.writeUInt8(1);
	packet.writeUInt32(1);
	packet.writeUInt16(1);

	uint32_t val = 1;
	visNET::BlobArray<uint32_t> blob;
	blob.add(val);

	packet.writeBlobArray(blob);

	EXPECT_TRUE(packet.isWritable());
	EXPECT_TRUE(packet.isValid());
}

TEST(PacketTest, SendAndReadPacketThroughTcpConnection)
{
	/* Create TCP listener and client */
	EXPECT_TRUE(visNET::startup()) << "Failed to startup";

	visNET::TcpListener tcpSvr(TESTPACKET_PORT1);
	EXPECT_TRUE(tcpSvr.isValid()) << "Failed to initialize TcpListener";

	visNET::TcpClient tcpCl("127.0.0.1", TESTPACKET_PORT1);
	EXPECT_TRUE(tcpCl.isValid()) << "Failed to initialize TcpClient";

	/* Create Test Packet */
	visNET::Packet packet;
	packet.writeBool(true);
	packet.writeInt8(-23);
	packet.writeDouble(31.5f);
	packet.writeFloat(1111.f);
	packet.writeInt32(144);
	packet.writeInt16(-4444);
	std::string testString = "Test";
	packet.writeString(testString);
	packet.writeUInt8(3);
	packet.writeUInt32(1);
	packet.writeUInt16(3);
	packet.writeUInt64(0xFFFFFFFFFFFF);
	packet.writeInt64(0xABCFFFFFFFFF);

	int32_t val = 1;
	visNET::BlobArray<int32_t> blob;
	blob.add(val);

	int32_t valArr[] = { 33, 34 };
	blob.add(valArr, 2);

	packet.writeBlobArray(blob);

	packet.writeInt16(33); // Skip test
	packet.writeInt16(34);

	/* Send Test Packet */
	tcpCl.send(packet);

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

	/* Receive Packet */
	auto recv = tcpSvr.receive();
	while (recv.empty())
	{
		EXPECT_NE(0, nAttempts--) << "Failed to receive a data packet";

		// Halt execution
		std::this_thread::sleep_for(std::chrono::microseconds(1));

		recv = tcpSvr.receive();
	}

	auto pPacket = recv.at(0).getPacket();
	EXPECT_TRUE(pPacket->isReadable());

	EXPECT_EQ(true, pPacket->readBool())						<< "Failed to read bool from packet";
	EXPECT_EQ(static_cast<int8_t>(-23), pPacket->readInt8())	<< "Failed to read int8 from packet";
	EXPECT_EQ(static_cast<double>(31.5f), pPacket->readDouble())<< "Failed to read double from packet";
	EXPECT_EQ(1111.f, pPacket->readFloat())						<< "Failed to read float from packet";
	EXPECT_EQ(144, pPacket->readInt32())						<< "Failed to read int32 from packet";
	EXPECT_EQ(static_cast<int16_t>(-4444), pPacket->readInt16())<< "Failed to read int16 from packet";
	EXPECT_EQ(testString, pPacket->readString())				<< "Failed to read string from packet";
	EXPECT_EQ(static_cast<uint8_t>(3), pPacket->readUInt8())	<< "Failed to read uint8 from packet";
	EXPECT_EQ(static_cast<uint32_t>(1), pPacket->readUInt32())	<< "Failed to read uint32 from packet";
	EXPECT_EQ(static_cast<int16_t>(3), pPacket->readUInt16())	<< "Failed to read uint16 from packet";

	if (0xFFFFFFFFFFFF != pPacket->readInt64())
		EXPECT_TRUE(false) << "Failed to read int64 from packet";

	if (0xABCFFFFFFFFF != pPacket->readUInt64())
		EXPECT_TRUE(false) << "Failed to read uint64 from packet";

	auto recvBlob = pPacket->readBlobArray<int32_t>();
	EXPECT_EQ(static_cast<uint32_t>(3), recvBlob->getItemCount()) << "Incorrect blob count";
	EXPECT_EQ(1, *recvBlob->get(0)) << "Blob value 1 is incorrect";
	EXPECT_EQ(33, *recvBlob->get(1)) << "Blob value 2 is incorrect";
	EXPECT_EQ(34, *recvBlob->get(2)) << "Blob value 3 is incorrect";

	EXPECT_TRUE(pPacket->readSkip(sizeof(int16_t))) << L"Failed to skip parts of packet";
	EXPECT_EQ(static_cast<int16_t>(34), pPacket->readInt16()) << "Failed to read uint16 after skip";

	/* Shutdown Winsock */
	EXPECT_TRUE(visNET::cleanup()) << "Failed to cleanup";
}

TEST(PacketTest, SendAndReadPacketThroughUdp)
{
	EXPECT_TRUE(visNET::startup()) << "Failed to startup";

	visNET::UdpClient udpClient1(TESTPACKET_PORT2);
	visNET::UdpClient udpClient2(TESTPACKET_PORT3);

	/* Send Packet */
	visNET::Packet packet;
	packet.writeBool(true);
	packet.writeInt8(-23);
	packet.writeDouble(31.5f);
	packet.writeFloat(1111.f);
	packet.writeInt32(144);
	packet.writeInt16(-4444);
	std::string testString = "Test";
	packet.writeString(testString);
	packet.writeUInt8(3);
	packet.writeUInt32(1);
	packet.writeUInt16(3);
	packet.writeUInt64(0xFFFFFFFFFFFF);
	packet.writeInt64(0xABCFFFFFFFFF);

	int32_t val = 1;
	visNET::BlobArray<int32_t> blob;
	blob.add(val);

	int32_t valArr[] = { 33, 34 };
	blob.add(valArr, 2);

	packet.writeBlobArray(blob);

	packet.writeInt16(33); // Skip test
	packet.writeInt16(34);

	udpClient1.send("127.0.0.1", TESTPACKET_PORT3, packet);

	int32_t nAttempts = 50;

	/* Receive Packet */
	auto recv = udpClient2.receive();
	while (recv.empty())
	{
		EXPECT_NE(0, nAttempts--) << "Failed to receive a data packet";

		// Halt execution
		std::this_thread::sleep_for(std::chrono::microseconds(1));

		recv = udpClient2.receive();
	}

	auto pPacket = recv.at(0).getPacket();
	EXPECT_TRUE(pPacket->isReadable());

	EXPECT_EQ(true, pPacket->readBool())						<< "Failed to read bool from packet";
	EXPECT_EQ(static_cast<int8_t>(-23), pPacket->readInt8())	<< "Failed to read int8 from packet";
	EXPECT_EQ(static_cast<double>(31.5f), pPacket->readDouble())<< "Failed to read double from packet";
	EXPECT_EQ(1111.f, pPacket->readFloat())						<< "Failed to read float from packet";
	EXPECT_EQ(144, pPacket->readInt32())						<< "Failed to read int32 from packet";
	EXPECT_EQ(static_cast<int16_t>(-4444), pPacket->readInt16())<< "Failed to read int16 from packet";
	EXPECT_EQ(testString, pPacket->readString())				<< "Failed to read string from packet";
	EXPECT_EQ(static_cast<uint8_t>(3), pPacket->readUInt8())	<< "Failed to read uint8 from packet";
	EXPECT_EQ(static_cast<uint32_t>(1), pPacket->readUInt32())	<< "Failed to read uint32 from packet";
	EXPECT_EQ(static_cast<int16_t>(3), pPacket->readUInt16())	<< "Failed to read uint16 from packet";

	if (0xFFFFFFFFFFFF != pPacket->readInt64())
		EXPECT_TRUE(false) << "Failed to read int64 from packet";

	if (0xABCFFFFFFFFF != pPacket->readUInt64())
		EXPECT_TRUE(false) << "Failed to read uint64 from packet";

	auto recvBlob = pPacket->readBlobArray<int32_t>();
	EXPECT_EQ(static_cast<uint32_t>(3), recvBlob->getItemCount()) << "Incorrect blob count";
	EXPECT_EQ(1, *recvBlob->get(0)) << "Blob value 1 is incorrect";
	EXPECT_EQ(33, *recvBlob->get(1)) << "Blob value 2 is incorrect";
	EXPECT_EQ(34, *recvBlob->get(2)) << "Blob value 3 is incorrect";

	EXPECT_TRUE(pPacket->readSkip(sizeof(int16_t))) << L"Failed to skip parts of packet";
	EXPECT_EQ(static_cast<int16_t>(34), pPacket->readInt16()) << "Failed to read uint16 after skip";

	/* Shutdown Winsock */
	EXPECT_TRUE(visNET::cleanup()) << "Failed to cleanup";
}

TEST(PacketTest, BlockReadingWritablePacket)
{
	visNET::Packet packet;
	EXPECT_TRUE(packet.isWritable());

	packet.writeInt8(43);

	EXPECT_TRUE(packet.isWritable());
	EXPECT_TRUE(packet.isValid());
	EXPECT_FALSE(packet.isReadable());

	int8_t val = packet.readInt8();
	EXPECT_EQ(static_cast<int8_t>(0), val);
}
#include "pch.h"

TEST(UdpTest, ConstructPeers)
{
	EXPECT_TRUE(visNET::startup()) << "Failed to startup";

	visNET::UdpClient udpClient1(TESTUDP_PORT3);
	visNET::UdpClient udpClient2(TESTUDP_PORT4);
	visNET::UdpClient udpClient3(TESTUDP_PORT5);

	// Shouldn't be valid
	visNET::UdpClient udpClient4(TESTUDP_PORT5);
	EXPECT_FALSE(udpClient4.isValid());

	/* Send from invalid peer */
	visNET::Packet packet;
	EXPECT_FALSE(udpClient4.send("127.0.0.1", TESTUDP_PORT3, packet));

	/* Send to peer udpClient3; Should not receive at peer udpClient4 */
	visNET::Packet packet2;
	EXPECT_TRUE(udpClient1.send("127.0.0.1", TESTUDP_PORT5, packet));

	/* Get received packet size from invalid peer */
	EXPECT_EQ(static_cast<size_t>(0), udpClient4.receive().size()) << "Invalid peer 4 received a packet";

	/* Get received packet size from valid peer */
	EXPECT_EQ(static_cast<size_t>(1), udpClient3.receive().size()) << "Valid peer 3 didn't receive a packet";

	/* Send to peer udpClient1 from peer udpClient2 */
	visNET::Packet packet3;
	EXPECT_TRUE(udpClient2.send("127.0.0.1", TESTUDP_PORT3, packet));

	/* Send to peer udpClient2 from peer udpClient3 */
	visNET::Packet packet4;
	EXPECT_TRUE(udpClient3.send("127.0.0.1", TESTUDP_PORT4, packet));

	/* Get received packet size from valid peer */
	EXPECT_EQ(static_cast<size_t>(1), udpClient1.receive().size()) << "Valid peer 1 didn't receive a packet";

	/* Get received packet size from valid peer */
	EXPECT_EQ(static_cast<size_t>(1), udpClient2.receive().size()) << "Valid peer 2 didn't receive a packet";

	/* Shutdown Winsock */
	EXPECT_TRUE(visNET::cleanup()) << "Failed to cleanup";
}

TEST(UdpTest, SendUdpPacket)
{
	EXPECT_TRUE(visNET::startup()) << "Failed to startup";

	visNET::UdpClient udpClient1(TESTUDP_PORT1);
	visNET::UdpClient udpClient2(TESTUDP_PORT2);

	/* Send Packet */
	visNET::Packet packet;
	packet.writeDouble(31.5f);
	udpClient1.send("127.0.0.1", TESTUDP_PORT2, packet);

	int32_t nAttempts = 50;

	/* Receive Packet */
	auto recv = udpClient2.receive();
	while (recv.empty())
	{
		EXPECT_NE(0, nAttempts--) << "Failed to receive a packet";

		// Halt execution
		std::this_thread::sleep_for(std::chrono::microseconds(1));

		recv = udpClient2.receive();
	}

	auto pPacket = recv.at(0).getPacket();
	EXPECT_TRUE(pPacket->isReadable());

	EXPECT_EQ(static_cast<double>(31.5f), pPacket->readDouble()) << "Failed to read double from packet";

	/* Shutdown Winsock */
	EXPECT_TRUE(visNET::cleanup()) << "Failed to cleanup";
}
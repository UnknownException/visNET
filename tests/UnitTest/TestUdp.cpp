#include "base.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest
{
	TEST_CLASS(TestUdp)
	{
	public:
		TEST_METHOD(ConstructPeers)
		{
			Assert::IsTrue(visNET::startup(), L"Failed to startup", LINE_INFO());

			visNET::UdpClient udpClient1(TESTUDP_PORT3);
			visNET::UdpClient udpClient2(TESTUDP_PORT4);
			visNET::UdpClient udpClient3(TESTUDP_PORT5);

			// Shouldn't be valid
			visNET::UdpClient udpClient4(TESTUDP_PORT5);
			Assert::IsFalse(udpClient4.isValid());

			/* Send from invalid peer */
			visNET::Packet packet;
			Assert::IsFalse(udpClient4.send("127.0.0.1", TESTUDP_PORT3, packet));

			/* Send to peer udpClient3; Should not receive at peer udpClient4 */
			visNET::Packet packet2;
			Assert::IsTrue(udpClient1.send("127.0.0.1", TESTUDP_PORT5, packet));
			
			/* Halt execution */
			Sleep(100);

			/* Get received packet size from invalid peer */
			Assert::AreEqual(static_cast<size_t>(0), udpClient4.getPackets().size(), L"Invalid peer 4 received a packet", LINE_INFO());

			/* Get received packet size from valid peer */
			Assert::AreEqual(static_cast<size_t>(1), udpClient3.getPackets().size(), L"Valid peer 3 didn't receive a packet", LINE_INFO());

			/* Send to peer udpClient1 from peer udpClient2 */
			visNET::Packet packet3;
			Assert::IsTrue(udpClient2.send("127.0.0.1", TESTUDP_PORT3, packet));

			/* Send to peer udpClient2 from peer udpClient3 */
			visNET::Packet packet4;
			Assert::IsTrue(udpClient3.send("127.0.0.1", TESTUDP_PORT4, packet));

			/* Halt execution */
			Sleep(100);

			/* Get received packet size from valid peer */
			Assert::AreEqual(static_cast<size_t>(1), udpClient1.getPackets().size(), L"Valid peer 1 didn't receive a packet", LINE_INFO());

			/* Get received packet size from valid peer */
			Assert::AreEqual(static_cast<size_t>(1), udpClient2.getPackets().size(), L"Valid peer 2 didn't receive a packet", LINE_INFO());

			/* Shutdown Winsock */
			Assert::IsTrue(visNET::cleanup(), L"Failed to cleanup", LINE_INFO());
		}

		TEST_METHOD(SendUdpPacket)
		{
			Assert::IsTrue(visNET::startup(), L"Failed to startup", LINE_INFO());

			visNET::UdpClient udpClient1(TESTUDP_PORT1);
			visNET::UdpClient udpClient2(TESTUDP_PORT2);
			
			/* Send Packet */
			visNET::Packet packet;
			packet.writeDouble(31.5f);
			udpClient1.send("127.0.0.1", TESTUDP_PORT2, packet);

			int32_t nAttempts = 50;

			/* Receive Packet */
			auto recv = udpClient2.getPackets();
			while (recv.empty())
			{
				recv = udpClient2.getPackets();

				// Halt Execution
				Sleep(10);
				nAttempts--;
				Assert::AreNotEqual(0, nAttempts, L"Failed to receive a data packet", LINE_INFO());
			}

			auto pPacket = recv.at(0).getPacket();
			Assert::IsTrue(pPacket->isReadable());

			Assert::AreEqual(static_cast<double>(31.5f), pPacket->readDouble(), L"Failed to read double from packet", LINE_INFO());

			/* Shutdown Winsock */
			Assert::IsTrue(visNET::cleanup(), L"Failed to cleanup", LINE_INFO());
		}
	};
}
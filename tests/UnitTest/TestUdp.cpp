#include "base.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest
{
	TEST_CLASS(TestUdp)
	{
	public:
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
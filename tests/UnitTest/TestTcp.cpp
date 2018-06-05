#include "base.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest
{
	TEST_CLASS(TestTcp)
	{
	public:
		/* Basic receiving and sending is tested inside TestPacket*/

		TEST_METHOD(ConstructConnection)
		{
			/* Create TCP listener and client */
			Assert::IsTrue(visNET::startup(), L"Failed to startup", LINE_INFO());

			visNET::TcpListener tcpSvr(TESTTCP_PORT1);
			Assert::IsTrue(tcpSvr.isValid(), L"Failed to initialize TcpListener", LINE_INFO());

			visNET::TcpClient tcpCl("127.0.0.1", TESTTCP_PORT1);
			Assert::IsTrue(tcpCl.isValid(), L"Failed to initialize TcpClient", LINE_INFO());

			/* Try to create second listener on the same port */
			visNET::TcpListener tcpSvrBad(TESTTCP_PORT1);
			Assert::IsFalse(tcpSvrBad.isValid(), L"Managed to create second listener on same port", LINE_INFO());

			/* Try to create a client that connects to a non existing listener */
			visNET::TcpClient tcpClBad("127.0.0.1", TESTTCP_PORT2);
			Assert::IsFalse(tcpClBad.isValid(), L"Managed to connect to an unknown listener", LINE_INFO());

			/* Try to create second listener on a different port */
			visNET::TcpListener tcpSvrSecond(TESTTCP_PORT2);
			Assert::IsTrue(tcpSvrSecond.isValid(), L"Couldn't create second listener", LINE_INFO());

			/* Try to connect to second listener */
			visNET::TcpClient tcpClSecond("127.0.0.1", TESTTCP_PORT2);
			Assert::IsTrue(tcpClSecond.isValid(), L"Failed to connect to second listener", LINE_INFO());
			
			/* Shutdown Winsock */
			Assert::IsTrue(visNET::cleanup(), L"Failed to cleanup", LINE_INFO());
		}

		TEST_METHOD(SendLotsOfPacketsToServer)
		{
			/* Create TCP listener and client */
			Assert::IsTrue(visNET::startup(), L"Failed to startup", LINE_INFO());

			visNET::TcpListener tcpSvr(TESTTCP_PORT3);
			Assert::IsTrue(tcpSvr.isValid(), L"Failed to initialize TcpListener", LINE_INFO());

			visNET::TcpClient tcpCl("127.0.0.1", TESTTCP_PORT3);
			Assert::IsTrue(tcpCl.isValid(), L"Failed to initialize TcpClient", LINE_INFO());

			/* Create and send test packet */
			for (uint32_t i = 0; i < 4096 * 8; ++i)
			{
				visNET::Packet packet;
				packet.writeUInt(i % 913);

				tcpCl.send(packet);
			}

			/* Receive attempts */
			int32_t nAttempts = 50;

			/* Accept Connection */
			auto connectionId = tcpSvr.getConnection();
			while (connectionId == 0)
			{
				connectionId = tcpSvr.getConnection();

				// Halt Execution
				Sleep(10);
				nAttempts--;
				Assert::AreNotEqual(0, nAttempts, L"Failed to establish a connection", LINE_INFO());
			}

			/* Reset attempts */
			nAttempts = 50;

			/* Try to receive all send packets */
			std::vector<uint32_t> received;
			while (received.size() < 4096 * 8)
			{
				auto recv = tcpSvr.getPackets();
				while (recv.empty())
				{
					recv = tcpSvr.getPackets();

					// Halt Execution
					Sleep(10);
					nAttempts--;
					Assert::AreNotEqual(0, nAttempts, L"Failed to receive a data packet", LINE_INFO());
				}

				for (auto it = recv.begin(); it != recv.end(); ++it)
					received.push_back((*it).getPacket()->readUInt());

				/* Reset attempts */
				nAttempts = 50;
			}

			/* Check received values */
			for (uint32_t i = 0; i < 4096 * 8; ++i)
				Assert::AreEqual(i % 913, received[i], L"Received value is different from expected value", LINE_INFO());

			/* Shutdown Winsock */
			Assert::IsTrue(visNET::cleanup(), L"Failed to cleanup", LINE_INFO());
		}

		TEST_METHOD(SendLotsOfPacketsToClient)
		{
			/* Create TCP listener and client */
			Assert::IsTrue(visNET::startup(), L"Failed to startup", LINE_INFO());

			visNET::TcpListener tcpSvr(TESTTCP_PORT4);
			Assert::IsTrue(tcpSvr.isValid(), L"Failed to initialize TcpListener", LINE_INFO());

			visNET::TcpClient tcpCl("127.0.0.1", TESTTCP_PORT4);
			Assert::IsTrue(tcpCl.isValid(), L"Failed to initialize TcpClient", LINE_INFO());

			/* Receive attempts */
			int32_t nAttempts = 50;

			/* Accept Connection */
			auto connectionId = tcpSvr.getConnection();
			while (connectionId == 0)
			{
				connectionId = tcpSvr.getConnection();

				// Halt Execution
				Sleep(10);
				nAttempts--;
				Assert::AreNotEqual(0, nAttempts, L"Failed to establish a connection", LINE_INFO());
			}

			/* Create and send test packet */
			for (uint32_t i = 0; i < 4096 * 8; ++i)
			{
				visNET::Packet packet;
				packet.writeUInt(i % 844);

				tcpSvr.send(connectionId, packet);
			}

			/* Reset attempts */
			nAttempts = 50;

			/* Try to receive all send packets */
			std::vector<uint32_t> received;
			while (received.size() < 4096 * 8)
			{
				auto recv = tcpCl.getPackets();
				while (recv.empty())
				{
					recv = tcpCl.getPackets();

					// Halt Execution
					Sleep(10);
					nAttempts--;
					Assert::AreNotEqual(0, nAttempts, L"Failed to receive a data packet", LINE_INFO());
				}

				for (auto it = recv.begin(); it != recv.end(); ++it)
					received.push_back((*it).getPacket()->readUInt());

				/* Reset attempts */
				nAttempts = 50;
			}

			/* Check received values */
			for (uint32_t i = 0; i < 4096 * 8; ++i)
				Assert::AreEqual(i % 844, received[i], L"Received value is different from expected value", LINE_INFO());

			/* Shutdown Winsock */
			Assert::IsTrue(visNET::cleanup(), L"Failed to cleanup", LINE_INFO());
		}
	};
}
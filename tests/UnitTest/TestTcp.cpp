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
				packet.writeUInt32(i % 913);

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
				auto recv = tcpSvr.receive();
				while (recv.empty())
				{
					recv = tcpSvr.receive();

					// Halt Execution
					Sleep(10);
					nAttempts--;
					Assert::AreNotEqual(0, nAttempts, L"Failed to receive a data packet", LINE_INFO());
				}

				for (auto it = recv.begin(); it != recv.end(); ++it)
					received.push_back((*it).getPacket()->readUInt32());

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
					recv = tcpCl.receive();

					// Halt Execution
					Sleep(10);
					nAttempts--;
					Assert::AreNotEqual(0, nAttempts, L"Failed to receive a data packet", LINE_INFO());
				}

				for (auto it = recv.begin(); it != recv.end(); ++it)
					received.push_back((*it).getPacket()->readUInt32());

				/* Reset attempts */
				nAttempts = 50;
			}

			/* Check received values */
			for (uint32_t i = 0; i < 4096 * 8; ++i)
				Assert::AreEqual(i % 844, received[i], L"Received value is different from expected value", LINE_INFO());

			/* Shutdown Winsock */
			Assert::IsTrue(visNET::cleanup(), L"Failed to cleanup", LINE_INFO());
		}

		TEST_METHOD(SendBigPacketsToServer)
		{
			/* Create TCP listener and client */
			Assert::IsTrue(visNET::startup(), L"Failed to startup", LINE_INFO());

			visNET::TcpListener tcpSvr(TESTTCP_PORT5);
			Assert::IsTrue(tcpSvr.isValid(), L"Failed to initialize TcpListener", LINE_INFO());

			visNET::TcpClient tcpCl("127.0.0.1", TESTTCP_PORT5);
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
					recv = tcpSvr.receive();

					// Halt Execution
					Sleep(10);
					nAttempts--;
					Assert::AreNotEqual(0, nAttempts, L"Failed to receive a data packet", LINE_INFO());
				}

				for (auto it = recv.begin(); it != recv.end(); ++it)
				{
					std::vector<uint8_t> params;
					for(uint32_t x = 0; x < packetSizeLimit; ++x)
						params.push_back((*it).getPacket()->readUInt8());

					received.push_back(params);
					Assert::AreEqual((size_t)packetSizeLimit, params.size(), L"Received different amount of parameters than expected", LINE_INFO());
				}

				/* Reset attempts */
				nAttempts = 50;
			}

			/* Check received values */
			for (uint32_t i = 0; i < packetsToSend; ++i)
			{
				for(uint32_t x = 0; x < packetSizeLimit; ++x)
					Assert::AreEqual(static_cast<uint8_t>(x % 256), received[i][x], L"Received value is different from expected value", LINE_INFO());
			}

			/* Shutdown Winsock */
			Assert::IsTrue(visNET::cleanup(), L"Failed to cleanup", LINE_INFO());
		}


		TEST_METHOD(SendBigPacketsToClient)
		{
			/* Create TCP listener and client */
			Assert::IsTrue(visNET::startup(), L"Failed to startup", LINE_INFO());

			visNET::TcpListener tcpSvr(TESTTCP_PORT6);
			Assert::IsTrue(tcpSvr.isValid(), L"Failed to initialize TcpListener", LINE_INFO());

			visNET::TcpClient tcpCl("127.0.0.1", TESTTCP_PORT6);
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
					recv = tcpCl.receive();

					// Halt Execution
					Sleep(10);
					nAttempts--;
					Assert::AreNotEqual(0, nAttempts, L"Failed to receive a data packet", LINE_INFO());
				}

				for (auto it = recv.begin(); it != recv.end(); ++it)
				{
					std::vector<uint8_t> params;
					for (uint32_t x = 0; x < packetSizeLimit; ++x)
						params.push_back((*it).getPacket()->readUInt8());

					received.push_back(params);
					Assert::AreEqual((size_t)packetSizeLimit, params.size(), L"Received different amount of parameters than expected", LINE_INFO());
				}

				/* Reset attempts */
				nAttempts = 50;
			}

			/* Check received values */
			for (uint32_t i = 0; i < packetsToSend; ++i)
			{
				for (uint32_t x = 0; x < packetSizeLimit; ++x)
					Assert::AreEqual(static_cast<uint8_t>(x % 256), received[i][x], L"Received value is different from expected value", LINE_INFO());
			}

			/* Shutdown Winsock */
			Assert::IsTrue(visNET::cleanup(), L"Failed to cleanup", LINE_INFO());
		}

		TEST_METHOD(TestTcpPerformance)
		{
			uint32_t curTime = timeGetTime();

			/* Create TCP listener and client */
			Assert::IsTrue(visNET::startup(), L"Failed to startup", LINE_INFO());

			visNET::TcpListener tcpSvr(TESTTCP_PORT7);
			Assert::IsTrue(tcpSvr.isValid(), L"Failed to initialize TcpListener", LINE_INFO());

			visNET::TcpClient tcpCl("127.0.0.1", TESTTCP_PORT7);
			Assert::IsTrue(tcpCl.isValid(), L"Failed to initialize TcpClient", LINE_INFO());

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
				connectionId = tcpSvr.getConnection();
				nAttempts--;
				Assert::AreNotEqual(0, nAttempts, L"Failed to establish a connection", LINE_INFO());
			}

			/* Reset attempts */
			nAttempts = 5000;

			/* Receive Packet */
			auto recv = tcpSvr.receive();
			while (recv.empty())
			{
				recv = tcpSvr.receive();
				nAttempts--;
				Assert::AreNotEqual(0, nAttempts, L"Failed to receive a data packet", LINE_INFO());
			}

			auto pPacket = recv.at(0).getPacket();
			Assert::IsTrue(pPacket->isReadable());

			Assert::AreEqual(static_cast<double>(31.5f), pPacket->readDouble(), L"Failed to read double from packet", LINE_INFO());

			// Sleep performance....
			Assert::IsFalse(curTime + 5 < timeGetTime(), L"Failed to deliver local packet in time", LINE_INFO());

			/* Shutdown Winsock */
			Assert::IsTrue(visNET::cleanup(), L"Failed to cleanup", LINE_INFO());
		}

		TEST_METHOD(DisconnectClient)
		{
			/* Create TCP listener and client */
			Assert::IsTrue(visNET::startup(), L"Failed to startup", LINE_INFO());

			visNET::TcpListener tcpSvr(TESTTCP_PORT8);
			Assert::IsTrue(tcpSvr.isValid(), L"Failed to initialize TcpListener", LINE_INFO());

			visNET::TcpClient tcpCl("127.0.0.1", TESTTCP_PORT8);
			Assert::IsTrue(tcpCl.isValid(), L"Failed to initialize TcpClient", LINE_INFO());

			int32_t nAttempts = 50;

			/* Accept Connection */
			auto connectionId = tcpSvr.getConnection();
			while (connectionId == 0)
			{
				connectionId = tcpSvr.getConnection();

				// Halt execution
				Sleep(1);
				nAttempts--;
				Assert::AreNotEqual(0, nAttempts, L"Failed to establish a connection", LINE_INFO());
			}

			tcpSvr.disconnect(connectionId);
			
			auto disconnected = tcpSvr.getDisconnected();
			while (disconnected.empty())
			{
				disconnected = tcpSvr.getDisconnected();

				Sleep(1);
				nAttempts--;
				Assert::AreNotEqual(0, nAttempts, L"Failed to disconnect a connection", LINE_INFO());
			}

			if(connectionId != disconnected.at(0))
				Assert::IsTrue(false, L"Disconnection id does not match", LINE_INFO());

			Assert::IsTrue(tcpCl.isDisconnected(), L"TCP client is giving the wrong state", LINE_INFO());

			/* Shutdown Winsock */
			Assert::IsTrue(visNET::cleanup(), L"Failed to cleanup", LINE_INFO());
		}
	};
}
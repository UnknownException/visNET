#include "base.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest
{
	TEST_CLASS(TestPacket)
	{
	public:
		TEST_METHOD(ConstructPacket)
		{
			visNET::Packet packet;
			Assert::IsTrue(packet.isWritable());

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

			Assert::IsTrue(packet.isWritable());
			Assert::IsTrue(packet.isValid());
		}

		TEST_METHOD(SendAndReadPacketThroughTcpConnection)
		{
			/* Create TCP listener and client */
			Assert::IsTrue(visNET::startup(), L"Failed to startup", LINE_INFO());

			visNET::TcpListener tcpSvr(TESTPACKET_PORT1);
			Assert::IsTrue(tcpSvr.isValid(), L"Failed to initialize TcpListener", LINE_INFO());

			visNET::TcpClient tcpCl("127.0.0.1", TESTPACKET_PORT1);
			Assert::IsTrue(tcpCl.isValid(), L"Failed to initialize TcpClient", LINE_INFO());

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
				connectionId = tcpSvr.getConnection();

				// Halt Execution
				Sleep(10);
				nAttempts--;
				Assert::AreNotEqual(0, nAttempts, L"Failed to establish a connection", LINE_INFO());
			}

			/* Reset attempts */
			nAttempts = 50;

			/* Receive Packet */
			auto recv = tcpSvr.getPackets();
			while (recv.empty())
			{
				recv = tcpSvr.getPackets();

				// Halt Execution
				Sleep(10);
				nAttempts--;
				Assert::AreNotEqual(0, nAttempts, L"Failed to receive a data packet", LINE_INFO());
			}

			auto pPacket = recv.at(0).getPacket();
			Assert::IsTrue(pPacket->isReadable());

			Assert::AreEqual(true, pPacket->readBool(), L"Failed to read bool from packet", LINE_INFO());
			Assert::AreEqual(static_cast<int8_t>(-23), pPacket->readInt8(), L"Failed to read int8 from packet", LINE_INFO());
			Assert::AreEqual(static_cast<double>(31.5f), pPacket->readDouble(), L"Failed to read double from packet", LINE_INFO());
			Assert::AreEqual(1111.f, pPacket->readFloat(), L"Failed to read float from packet", LINE_INFO());
			Assert::AreEqual(144, pPacket->readInt32(), L"Failed to read int32 from packet", LINE_INFO());
			Assert::AreEqual(static_cast<int16_t>(-4444), pPacket->readInt16(), L"Failed to read int16 from packet", LINE_INFO());
			Assert::AreEqual(testString, pPacket->readString(), L"Failed to read string from packet", LINE_INFO());
			Assert::AreEqual(static_cast<uint8_t>(3), pPacket->readUInt8(), L"Failed to read uint8 from packet", LINE_INFO());
			Assert::AreEqual(static_cast<uint32_t>(1), pPacket->readUInt32(), L"Failed to uint32 from packet", LINE_INFO());
			// Casting to int16_t; MSVC++ UnitTest uses unsigned short for wide character strings
			Assert::AreEqual(static_cast<int16_t>(3), static_cast<int16_t>(pPacket->readUInt16()), L"Failed to uint16 from packet", LINE_INFO());

			if (0xFFFFFFFFFFFF != pPacket->readInt64())
				Assert::Fail(L"Failed to read int64 from packet", LINE_INFO());

			if (0xABCFFFFFFFFF != pPacket->readUInt64())
				Assert::Fail(L"Failed to read uint64 from packet", LINE_INFO());

			auto recvBlob = pPacket->readBlobArray<int32_t>();
			Assert::AreEqual(static_cast<uint32_t>(3), recvBlob->getBlobCount(), L"Incorrect blob count", LINE_INFO());
			Assert::AreEqual(1, *recvBlob->get(0), L"Blob value 1 is incorrect", LINE_INFO());
			Assert::AreEqual(33, *recvBlob->get(1), L"Blob value 2 is incorrect", LINE_INFO());
			Assert::AreEqual(34, *recvBlob->get(2), L"Blob value 3 is incorrect", LINE_INFO());

			Assert::AreEqual(true, pPacket->readSkip(sizeof(int16_t)), L"Failed to skip parts of packet", LINE_INFO());
			Assert::AreEqual(static_cast<int16_t>(34), pPacket->readInt16(), L"Failed to read uint16 after skip", LINE_INFO());

			/* Shutdown Winsock */
			Assert::IsTrue(visNET::cleanup(), L"Failed to cleanup", LINE_INFO());
		}

		TEST_METHOD(SendAndReadPacketThroughUdp)
		{
			Assert::IsTrue(visNET::startup(), L"Failed to startup", LINE_INFO());

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

			Assert::AreEqual(true, pPacket->readBool(), L"Failed to read bool from packet", LINE_INFO());
			Assert::AreEqual(static_cast<int8_t>(-23), pPacket->readInt8(), L"Failed to read int8 from packet", LINE_INFO());
			Assert::AreEqual(static_cast<double>(31.5f), pPacket->readDouble(), L"Failed to read double from packet", LINE_INFO());
			Assert::AreEqual(1111.f, pPacket->readFloat(), L"Failed to read float from packet", LINE_INFO());
			Assert::AreEqual(144, pPacket->readInt32(), L"Failed to read int32 from packet", LINE_INFO());
			Assert::AreEqual(static_cast<int16_t>(-4444), pPacket->readInt16(), L"Failed to read int16 from packet", LINE_INFO());
			Assert::AreEqual(testString, pPacket->readString(), L"Failed to read string from packet", LINE_INFO());
			Assert::AreEqual(static_cast<uint8_t>(3), pPacket->readUInt8(), L"Failed to read uint8 from packet", LINE_INFO());
			Assert::AreEqual(static_cast<uint32_t>(1), pPacket->readUInt32(), L"Failed to uint32 from packet", LINE_INFO());
			// Casting to int16_t; MSVC++ UnitTest uses unsigned short for wide character strings
			Assert::AreEqual(static_cast<int16_t>(3), static_cast<int16_t>(pPacket->readUInt16()), L"Failed to uint16 from packet", LINE_INFO());

			if (0xFFFFFFFFFFFF != pPacket->readInt64())
				Assert::Fail(L"Failed to read int64 from packet", LINE_INFO());

			if (0xABCFFFFFFFFF != pPacket->readUInt64())
				Assert::Fail(L"Failed to read uint64 from packet", LINE_INFO());

			auto recvBlob = pPacket->readBlobArray<int32_t>();
			Assert::AreEqual(static_cast<uint32_t>(3), recvBlob->getBlobCount(), L"Incorrect blob count", LINE_INFO());
			Assert::AreEqual(1, *recvBlob->get(0), L"Blob value 1 is incorrect", LINE_INFO());
			Assert::AreEqual(33, *recvBlob->get(1), L"Blob value 2 is incorrect", LINE_INFO());
			Assert::AreEqual(34, *recvBlob->get(2), L"Blob value 3 is incorrect", LINE_INFO());

			Assert::AreEqual(true, pPacket->readSkip(sizeof(int16_t)), L"Failed to skip parts of packet", LINE_INFO());
			Assert::AreEqual(static_cast<int16_t>(34), pPacket->readInt16(), L"Failed to read uint16 after skip", LINE_INFO());

			/* Shutdown Winsock */
			Assert::IsTrue(visNET::cleanup(), L"Failed to cleanup", LINE_INFO());
		}

		TEST_METHOD(BlockReadingWritablePacket)
		{
			visNET::Packet packet;
			Assert::IsTrue(packet.isWritable());

			packet.writeInt8(43);

			Assert::IsTrue(packet.isWritable());
			Assert::IsTrue(packet.isValid());
			Assert::IsFalse(packet.isReadable());

			int8_t val = packet.readInt8();
			Assert::AreEqual(static_cast<int8_t>(0), val);
		}
	};
}
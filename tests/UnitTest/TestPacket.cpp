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
			packet.writeChar(1);
			packet.writeDouble(1.f);
			packet.writeFloat(1.f);
			packet.writeInt(1);
			packet.writeShort(1);
			packet.writeString("Test");
			packet.writeUChar(1);
			packet.writeUInt(1);
			packet.writeUShort(1);

			uint32_t val = 1;
			visNET::BlobArray* pBlob = new visNET::BlobArray(sizeof(uint32_t));
			pBlob->add(&val);

			packet.writeBlobArray(*pBlob);

			Assert::IsTrue(packet.isWritable());
			Assert::IsTrue(packet.isValid());
		}

		TEST_METHOD(SendAndReadPacketThroughTcpConnection)
		{
			/* Create TCP listener and client */
			Assert::IsTrue(visNET::startup(), L"Failed to startup", LINE_INFO());

			visNET::TcpListener tcpSvr(6123);
			Assert::IsTrue(tcpSvr.isValid(), L"Failed to initialize TcpListener", LINE_INFO());

			visNET::TcpClient tcpCl("127.0.0.1", 6123);
			Assert::IsTrue(tcpCl.isValid(), L"Failed to initialize TcpClient", LINE_INFO());

			/* Create Test Packet */
			visNET::Packet packet;
			packet.writeBool(true);
			packet.writeChar(-23);
			packet.writeDouble(31.5f);
			packet.writeFloat(1111.f);
			packet.writeInt(144);
			packet.writeShort(-4444);
			std::string testString = "Test";
			packet.writeString(testString);
			packet.writeUChar(0);
			packet.writeUInt(1);
			packet.writeUShort(0);

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

			auto pPacket = recv.at(0).second;
			Assert::IsTrue(pPacket->isReadable());

			Assert::AreEqual(true, pPacket->readBool(), L"Failed to read bool from packet", LINE_INFO());
			Assert::AreEqual(static_cast<int8_t>(-23), pPacket->readChar(), L"Failed to read char from packet", LINE_INFO());
			Assert::AreEqual(static_cast<double>(31.5f), pPacket->readDouble(), L"Failed to read double from packet", LINE_INFO());
			Assert::AreEqual(1111.f, pPacket->readFloat(), L"Failed to read float from packet", LINE_INFO());
			Assert::AreEqual(144, pPacket->readInt(), L"Failed to read int from packet", LINE_INFO());
			Assert::AreEqual(static_cast<int16_t>(-4444), pPacket->readShort(), L"Failed to read short from packet", LINE_INFO());
			Assert::AreEqual(testString, pPacket->readString(), L"Failed to read string from packet", LINE_INFO());
			Assert::AreEqual(static_cast<uint8_t>(0), pPacket->readUChar(), L"Failed to read unsigned char from packet", LINE_INFO());
			Assert::AreEqual(static_cast<uint32_t>(1), pPacket->readUInt(), L"Failed to unsigned int from packet", LINE_INFO());
			// Casting to int16_t; MSVC++ UnitTest uses unsigned short for wide character strings
			Assert::AreEqual(static_cast<int16_t>(0), static_cast<int16_t>(packet.readUShort()), L"Failed to unsigned short from packet", LINE_INFO());

			/* Shutdown Winsock */
			Assert::IsTrue(visNET::cleanup(), L"Failed to cleanup", LINE_INFO());
		}

		TEST_METHOD(BlockReadingWritablePacket)
		{
			visNET::Packet packet;
			Assert::IsTrue(packet.isWritable());

			packet.writeChar(43);

			Assert::IsTrue(packet.isWritable());
			Assert::IsTrue(packet.isValid());
			Assert::IsFalse(packet.isReadable());

			int8_t val = packet.readChar();
			Assert::AreEqual(static_cast<int8_t>(0), val);
		}
	};
}
#pragma once
#include <stdint.h>
#include <memory>
#include <tuple>
#include <string>
#include <vector>
#include "../../source/blobarray.h"

//template class __declspec(dllexport) std::shared_ptr<void>;

namespace visNET {
	/* Base Interface */
	__declspec(dllexport) bool startup();
	__declspec(dllexport) bool cleanup();
	
	/* Packet Interface */
	class __declspec(dllexport) Packet {
		friend class TcpClient;
		friend class TcpListener;
		friend class UdpClient;
		friend class PacketInstance;
		
		void* packetInstance;
	protected:
		void* getInstance() { return packetInstance; };

	public:
		Packet();
		// Do not use this constructor, it's for internal purpose
		Packet(std::shared_ptr<void> instance);
		virtual ~Packet();

		void writeInt(int32_t n);
		void writeUInt(uint32_t n);
		void writeShort(int16_t n);
		void writeUShort(uint16_t n);
		void writeChar(int8_t n);
		void writeUChar(uint8_t n);
		void writeFloat(float f);
		void writeDouble(double d);
		void writeBool(bool b);
		void writeString(const char* str);
		void writeBlobArray(BlobArray& blob);

		bool readSkip(uint32_t offset);

		int32_t readInt();
		uint32_t readUInt();
		int16_t readShort();
		uint16_t readUShort();
		int8_t readChar();
		uint8_t readUChar();
		float readFloat();
		double readDouble();
		bool readBool();
		std::string readString();
		bool readBlobArray(BlobArray& blob);

		bool isReadable();
		bool isValid();
		bool isWritable();
		bool isTransfering();
	};

	/* Tcp Listener Interface */
	class __declspec(dllexport) TcpListener {
		void* tcpListener;
	public:
		TcpListener(uint16_t nPort);
		virtual ~TcpListener();

		uint32_t getConnection();

		void send(uint32_t nClId, Packet& packet);
		std::vector<std::pair<uint32_t, std::shared_ptr<Packet>>> getPackets();
		std::vector<uint32_t> getDisconnected();
		void disconnect(uint32_t nClId);

		bool isValid();
		std::string getError();
	};

	/* Tcp Client Interface */
	class __declspec(dllexport) TcpClient{
		void* tcpClient;

	public:
		TcpClient(const char* pszIp, uint16_t nPort);
		virtual ~TcpClient();

		void send(Packet& pPacket);
		std::vector<std::pair<uint32_t, std::shared_ptr<Packet>>> getPackets();
		bool isDisconnected();

		bool isValid();
		std::string getError();
	};

	/* Udp Client Interface */
	class __declspec(dllexport) UdpClient {
		void* udpClient;

	public:
		UdpClient(uint16_t nPort);
		virtual ~UdpClient();

		bool send(Packet& pPacket, const char* szIP, unsigned short nPort);
		std::vector<std::tuple<std::string, uint16_t, std::shared_ptr<Packet>>> getPackets();

		bool isValid();
		std::string getError();
	};
}
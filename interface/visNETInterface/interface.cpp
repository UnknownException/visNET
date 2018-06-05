#include "include.h"
#include "../../source/visnet.h"

namespace visNET {
	class PacketInstance {
		std::shared_ptr<visNETCore::Packet> packet;
	public:
		PacketInstance(std::shared_ptr<visNETCore::Packet> instance) {
			packet = instance;
		}

		virtual ~PacketInstance() {
		}

		std::shared_ptr<visNETCore::Packet> get() { return packet; }

		static std::shared_ptr<visNETCore::Packet> getInstance(visNET::Packet* packet) {
			return ((PacketInstance*)packet->getInstance())->get();
		}
	};

	bool startup()
	{
		return visNETCore::startup();
	}

	bool cleanup()
	{
		return visNETCore::cleanup();
	}

	/* Packet Interface */
	Packet::Packet()
	{
		packetInstance = new PacketInstance(std::make_shared<visNETCore::Packet>());
	}

	Packet::Packet(std::shared_ptr<void> instance)
	{
		packetInstance = new PacketInstance(std::static_pointer_cast<visNETCore::Packet>(instance));
	}

	Packet::~Packet()
	{
		delete (PacketInstance*)packetInstance;
	}

	void Packet::writeInt(int32_t n)
	{
		PacketInstance::getInstance(this)->writeInt(n);
	}

	void Packet::writeUInt(uint32_t n)
	{
		PacketInstance::getInstance(this)->writeUInt(n);
	}

	void Packet::writeShort(int16_t n)
	{
		PacketInstance::getInstance(this)->writeShort(n);
	}

	void Packet::writeUShort(uint16_t n)
	{
		PacketInstance::getInstance(this)->writeUShort(n);
	}

	void Packet::writeChar(int8_t n)
	{
		PacketInstance::getInstance(this)->writeChar(n);
	}

	void Packet::writeUChar(uint8_t n)
	{
		PacketInstance::getInstance(this)->writeUChar(n);
	}

	void Packet::writeFloat(float f)
	{
		PacketInstance::getInstance(this)->writeFloat(f);
	}

	void Packet::writeDouble(double d)
	{
		PacketInstance::getInstance(this)->writeDouble(d);
	}

	void Packet::writeBool(bool b)
	{
		PacketInstance::getInstance(this)->writeBool(b);
	}

	void Packet::writeString(const char* str) 
	{
		PacketInstance::getInstance(this)->writeString(str);
	}

	void Packet::writeBlobArray(BlobArray& blob)
	{
		PacketInstance::getInstance(this)->writeBlobArray(blob);
	}

	bool Packet::readSkip(uint32_t offset)
	{
		return PacketInstance::getInstance(this)->readSkip(offset);
	}

	int32_t Packet::readInt()
	{
		return PacketInstance::getInstance(this)->readInt();
	}

	uint32_t Packet::readUInt()
	{
		return PacketInstance::getInstance(this)->readUInt();
	}
	int16_t Packet::readShort()
	{
		return PacketInstance::getInstance(this)->readShort();
	}

	uint16_t Packet::readUShort()
	{
		return PacketInstance::getInstance(this)->readUShort();
	}

	int8_t Packet::readChar()
	{
		return PacketInstance::getInstance(this)->readChar();
	}

	uint8_t Packet::readUChar()
	{
		return PacketInstance::getInstance(this)->readUChar();
	}

	float Packet::readFloat()
	{
		return PacketInstance::getInstance(this)->readFloat();
	}

	double Packet::readDouble()
	{
		return PacketInstance::getInstance(this)->readDouble();
	}

	bool Packet::readBool()
	{
		return PacketInstance::getInstance(this)->readBool();
	}

	std::string Packet::readString()
	{
		return PacketInstance::getInstance(this)->readString();
	}

	bool Packet::readBlobArray(BlobArray& blob)
	{
		return PacketInstance::getInstance(this)->readBlobArray(blob);
	}

	bool Packet::isReadable()
	{
		return PacketInstance::getInstance(this)->isReadable();
	}

	bool Packet::isValid()
	{
		return PacketInstance::getInstance(this)->isValid();
	}

	bool Packet::isWritable()
	{
		return PacketInstance::getInstance(this)->isWritable();
	}

	bool Packet::isTransfering()
	{
		return PacketInstance::getInstance(this)->isTransfering();
	}

	/*
		TCP Listener Interface
	*/

	TcpListener::TcpListener(uint16_t nPort) 
	{
		tcpListener = new visNETCore::TcpListener(nPort);
	}
	
	TcpListener::~TcpListener()
	{
		delete tcpListener;
	}

	uint32_t TcpListener::getConnection()
	{
		return ((visNETCore::TcpListener*)tcpListener)->getConnection().getValue();
	}

	void TcpListener::send(uint32_t nClId, Packet& packet)
	{
		((visNETCore::TcpListener*)tcpListener)->send(visNETCore::TcpMessage(visNETCore::ConnectionIdentifier(nClId), PacketInstance::getInstance(&packet)));
	}

	std::vector<std::pair<uint32_t, std::shared_ptr<Packet>>> TcpListener::getPackets()
	{
		std::vector<std::pair<uint32_t, std::shared_ptr<Packet>>> retn;

		auto packets = ((visNETCore::TcpListener*)tcpListener)->getPackets();
		for (auto it = packets.begin(); it != packets.end(); ++it)
			retn.push_back(std::make_pair((*it).getConnectionIdentifier().getValue(), std::make_shared<Packet>((*it).getPacket())));

		return retn;
	}

	std::vector<uint32_t> TcpListener::getDisconnected()
	{
		std::vector<uint32_t> retn;

		auto disconnected = ((visNETCore::TcpListener*)tcpListener)->getDisconnected();
		for (auto it = disconnected.begin(); it != disconnected.end(); ++it)
			retn.push_back((*it).getValue());

		return retn;
	}

	void TcpListener::disconnect(uint32_t nClId)
	{
		return ((visNETCore::TcpListener*)tcpListener)->disconnect(nClId);
	}

	bool TcpListener::isValid()
	{
		return ((visNETCore::TcpListener*)tcpListener)->isValid();
	}

	std::string TcpListener::getError()
	{
		return  ((visNETCore::TcpListener*)tcpListener)->getError();
	}


	/*
		TCP Client Interface
	*/

	TcpClient::TcpClient(const char* pszIp, uint16_t nPort)
	{
		tcpClient = new visNETCore::TcpClient(pszIp, nPort);
	}

	TcpClient::~TcpClient()
	{
		delete tcpClient;
	}

	void TcpClient::send(Packet& packet)
	{
		((visNETCore::TcpClient*)tcpClient)->send(PacketInstance::getInstance(&packet));
	}

	std::vector<std::pair<uint32_t, std::shared_ptr<Packet>>> TcpClient::getPackets()
	{
		std::vector<std::pair<uint32_t, std::shared_ptr<Packet>>> retn;

		auto packets = ((visNETCore::TcpClient*)tcpClient)->getPackets();
		for (auto it = packets.begin(); it != packets.end(); ++it)
			retn.push_back(std::make_pair((*it).getConnectionIdentifier().getValue(), std::make_shared<Packet>((*it).getPacket())));

		return retn;
	}

	bool TcpClient::isDisconnected()
	{
		return ((visNETCore::TcpClient*)tcpClient)->isDisconnected();
	}

	bool TcpClient::isValid()
	{
		return ((visNETCore::TcpClient*)tcpClient)->isValid();
	}

	std::string TcpClient::getError()
	{
		return  ((visNETCore::TcpClient*)tcpClient)->getError();
	}

	/* 
		UDP Client Interface
	*/

	UdpClient::UdpClient(uint16_t nPort)
	{
		udpClient = new visNETCore::UdpClient(nPort);
	}

	UdpClient::~UdpClient()
	{
		delete udpClient;
	}

	bool UdpClient::send(Packet& packet, const char* szIP, unsigned short nPort)
	{
		return ((visNETCore::UdpClient*)udpClient)->send(visNETCore::UdpMessage(szIP, nPort, PacketInstance::getInstance(&packet)));
	}

	std::vector<std::tuple<std::string, uint16_t, std::shared_ptr<Packet>>> UdpClient::getPackets()
	{
		std::vector<std::tuple<std::string, uint16_t, std::shared_ptr<Packet>>> retn;

		auto packets = ((visNETCore::UdpClient*)udpClient)->getPackets();
		for (auto it = packets.begin(); it != packets.end(); ++it)
			retn.push_back(std::make_tuple((*it).getIP(), (*it).getPort(), std::make_shared<Packet>((*it).getPacket())));

		return retn;
	}

	bool UdpClient::isValid()
	{
		return ((visNETCore::UdpClient*)udpClient)->isValid();
	}

	std::string UdpClient::getError()
	{
		return  ((visNETCore::UdpClient*)udpClient)->getError();
	}
}
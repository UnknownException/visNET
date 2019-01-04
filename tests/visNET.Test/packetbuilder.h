#pragma once

/*
	PacketBuilder helps creating a packet as if it was received by network
*/

class PacketBuilder{
	std::vector<uint8_t> m_vData;

	PacketBuilder createBuilder(){
		PacketBuilder builder;
		builder.m_vData = m_vData;
		return builder;
	}

	template<typename T>
	void write(T value) {		
		uint8_t* pData = reinterpret_cast<uint8_t*>(&value);
		m_vData.insert(m_vData.end(), pData, pData + (sizeof(T)));
	}

public:
	PacketBuilder() {
		uint32_t packetSize = 0;
		uint8_t* pData = reinterpret_cast<uint8_t*>(&packetSize);
		m_vData.insert(m_vData.end(), pData, pData + sizeof(packetSize));
	}

	virtual ~PacketBuilder() {
	}

	PacketBuilder writeInt8(int8_t n)	{ write(n); return createBuilder(); }
	PacketBuilder writeInt16(int16_t n) { write(n); return createBuilder(); }
	PacketBuilder writeInt32(int32_t n) { write(n); return createBuilder(); }
	PacketBuilder writeInt64(int32_t n) { write(n); return createBuilder(); }
	PacketBuilder writeUInt8(uint8_t n) { write(n); return createBuilder(); }
	PacketBuilder writeUInt16(uint16_t n) { write(n); return createBuilder(); }
	PacketBuilder writeUInt32(uint32_t n) { write(n); return createBuilder(); }
	PacketBuilder writeUInt64(uint32_t n) { write(n); return createBuilder(); }

	std::shared_ptr<visNET::Packet> build() {
		std::shared_ptr<visNET::Packet> packet = std::make_shared<visNET::Packet>();

		uint32_t packetSize = m_vData.size();
		memcpy(&m_vData[0], &packetSize, sizeof(uint32_t));

		packet->_onReceive(&m_vData[0], m_vData.size());
		return packet;
	}

	std::shared_ptr<visNET::Packet> buildIncomplete() {
		std::shared_ptr<visNET::Packet> packet = std::make_shared<visNET::Packet>();

		uint32_t packetSize = m_vData.size() + 4;
		memcpy(&m_vData[0], &packetSize, sizeof(uint32_t));

		packet->_onReceive(&m_vData[0], m_vData.size());
		return packet;
	}
};
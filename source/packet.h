#pragma once

namespace visNET{
	class Packet : public RawPacket{
		uint32_t m_nPacketId;
	public:
		Packet();
		Packet(uint32_t packetId);
		virtual ~Packet();

		void encrypt();
		void decrypt();

		virtual void onReceive(uint8_t* pData, uint32_t nLength) override;
		virtual void onSend() override;
	};
}

#pragma once

namespace visNET{
	class Packet : public RawPacket{
		uint32_t m_nPacketId;
	public:
		Packet(uint32_t packetId);
		virtual ~Packet();

		virtual void finalize(); //Prepare packet for sending

		void encrypt();
		void decrypt();
	};
}

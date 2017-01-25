#pragma once

namespace visNET{
	class Packet : public RawPacket{
		uint32_t m_nPacketId;

	public:
		Packet();
		Packet(uint32_t packetId);
		virtual ~Packet();

	private:
		virtual void encrypt(); //If onsend is not overrided, you can decide to override this function from a derived packet instead 
		virtual bool decrypt(); //If onreceive is not overrided, you can decide to override this function from a derived packet instead 

	public:
		virtual bool _onReceive(uint8_t* pData, uint32_t nLength) override;
		virtual void _onSend() override;
	};
}

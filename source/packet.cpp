#include "visnet.h"
#include "rawpacket.h"
#include "packet.h"

namespace visNET{
	Packet::Packet()
	{
		m_nPacketId = 0;
		writeUInt(m_nPacketId); //First 4 bytes
		writeUInt(0); //Checksum placeholder
	}

	Packet::Packet(uint32_t nPacketID)
	{
		m_nPacketId = nPacketID;
		writeUInt(m_nPacketId); //First 4 bytes
		writeUInt(0); //Checksum placeholder
	}

	Packet::~Packet()
	{
	}

	void Packet::encrypt()
	{
		uint8_t* pData = const_cast<uint8_t*>(_getRawData());
		//The checksum is important for the Packets UDP implementation. 
		//If the package is damaged, it'll be noticable by verifying the checksum.
		uint32_t nChecksum = 0; 
		
		//This block under here is to obfuscate the packet content
		//Without obfuscation it will be very easy to analyze the packets with external tools
		//Warning: This obfuscation is fairly weak

		//Obfuscate packet id
		uint32_t nVal = 0;
		memcpy(&nVal, pData, sizeof(uint32_t));
		nVal ^= 0xAE;
		memcpy(pData, &nVal, sizeof(uint32_t));

		//Obfuscate packet content
		for (uint32_t i = (sizeof(uint32_t) /* Packet Id */ + sizeof(uint32_t) /* Checksum */); i < _getRawSize(); ++i)
		{
			nChecksum += pData[i];
			pData[i] ^= static_cast<uint8_t>(m_nPacketId); //Only need the lower end of the value
		}

		//Obfuscate generated checksum
		for (uint32_t i = 0; i < sizeof(uint32_t); ++i)
		{
			uint8_t* pVal = reinterpret_cast<uint8_t*>(&nChecksum);
			pVal[i] ^= static_cast<uint8_t>(m_nPacketId >> (2 + i));
		}

		// Overwrite checksum placeholder in packet data
		memcpy(pData + sizeof(uint32_t), &nChecksum, sizeof(uint32_t));
	}

	bool Packet::decrypt()
	{
		//Advice: Read the comments in the encrypt function

		uint8_t* pData = const_cast<uint8_t*>(_getRawData());
		uint32_t nChecksum = 0;

		// Obfuscate packet id
		uint32_t nVal = 0;
		memcpy(&nVal, pData, sizeof(uint32_t)); //Get ID
		nVal ^= 0xAE;
		memcpy(pData, &nVal, sizeof(uint32_t));

		// Obfuscate packet content
		for (uint32_t i = (sizeof(uint32_t) /* Packet Id */ + sizeof(uint32_t) /* Checksum */); i < _getRawSize(); ++i)
		{
			pData[i] ^= static_cast<uint8_t>(nVal); //Only need the lower end of the value
			nChecksum += pData[i]; // Make checksum based on unobfuscated data
		}

		// Obfuscate comparison checksum checksum
		for (uint32_t i = 0; i < sizeof(uint32_t); ++i)
		{
			uint8_t* pVal = reinterpret_cast<uint8_t*>(&nChecksum);
			pVal[i] ^= static_cast<uint8_t>(nVal >> (2 + i));
		}

		// Compare checksums
		if (memcmp(&nChecksum, pData + sizeof(uint32_t), sizeof(uint32_t)) != 0)
			return false;

		return true;
	}

	bool Packet::_onReceive(uint8_t* pData, uint32_t nLength)
	{
		if (RawPacket::_onReceive(pData, nLength))
			return false;

		// If we can't decrypt the packet, it is invalid.
		if (!decrypt())
			return false;

		uint8_t* pPacketData = const_cast<uint8_t*>(_getRawData());
		memcpy(&m_nPacketId, pPacketData, sizeof(uint32_t));

		return true;
	}

	void Packet::_onSend()
	{
		RawPacket::_onSend();

		encrypt();
	}
}
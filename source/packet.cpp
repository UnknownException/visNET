#include "visnet.h"
#include "rawpacket.h"
#include "packet.h"

namespace visNET{
	Packet::Packet(uint32_t nPacketID)
	{
		m_nPacketId = nPacketID;
		writeUInt(nPacketID); //First 4 bytes
		writeUInt(0); //Checksum placeholder
	}

	Packet::~Packet()
	{
	}

	void Packet::finalize()
	{
		encrypt();
	}

	void Packet::encrypt()
	{
		//The checksum is important for the Packets UDP implementation. 
		//If the package is damaged, it'll be noticable by verifying the checksum.
		uint32_t nChecksum = 0; 
		
		//This block under here is to obfuscate the packet content
		//Without obfuscation it will be very easy to analyze the packets with external tools
		//Warning: This obfuscation is fairly weak
		uint32_t nVal = 0;
		memcpy(&nVal, m_pData, sizeof(uint32_t)); //Get ID
		nVal ^= 0xAE;
		memcpy(m_pData, &nVal, sizeof(uint32_t));

		for (uint32_t i = (sizeof(uint32_t) /* Packet Id */ + sizeof(uint32_t) /* Checksum */); i < m_nSize; ++i)
		{
			nChecksum += m_pData[i];
			m_pData[i] ^= static_cast<uint8_t>(m_nPacketId); //Only need the lower end of the value
		}

		for (uint32_t i = 0; i < sizeof(uint32_t); ++i)
		{
			uint8_t* pVal = reinterpret_cast<uint8_t*>(&nChecksum);
			pVal[i] ^= static_cast<uint8_t>(m_nPacketId >> (2 + i));
		}

		memcpy(m_pData + sizeof(uint32_t), &nChecksum, sizeof(uint32_t)); //Overwrite checksum placeholder
	}

	void Packet::decrypt()
	{
		//Advice: Read the comments in the encrypt function
	}
}
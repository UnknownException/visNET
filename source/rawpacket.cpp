#include "visnet.h"
#include "rawpacket.h"

namespace visNET{
	RawPacket::RawPacket()
	{
		m_nSize = 0;
		m_pData = nullptr;

		//Not used when building a packet...
		m_nRead = 0;
		m_bValidRead = false; // Will block further reading if the content that is being read is invalid
	}

	RawPacket::~RawPacket()
	{
		if (m_pData)
			delete[] m_pData;
	}

	void RawPacket::write(const char* pData, uint32_t nLength)
	{
		if (!m_pData)
			m_pData = new uint8_t[nLength];
		else
			m_pData = (uint8_t*)realloc(m_pData, m_nSize + nLength);

		memcpy(m_pData + m_nSize, pData, nLength);
		m_nSize += nLength;
	}

	bool RawPacket::read(char* pBuffer, uint32_t nSize)
	{
		if (!m_bValidRead)
			return false;

		if (!m_pData || m_nRead + nSize > m_nSize) // Can't read without data or outside bounds
		{
			m_bValidRead = false;
			return false;
		}

		memcpy(pBuffer, m_pData + m_nRead, nSize);

		m_nRead += nSize;

		return true;
	}

	bool RawPacket::readSkip(uint32_t nOffset)
	{
		if (!m_bValidRead)
			return false;

		if (!m_pData || m_nRead + nOffset > m_nSize) // Can't read without data or outside of bounds
		{
			m_bValidRead = false;
			return false;
		}

		m_nRead += nOffset;

		return true;
	}

	void RawPacket::writeString(const char* str)
	{
		uint32_t nLen = strlen(str);
		
		uint8_t* pBuffer = new uint8_t[nLen];
		memcpy(pBuffer, str, nLen);

		writeUInt(nLen);
		write(reinterpret_cast<char*>(pBuffer), nLen);
	}

	void RawPacket::writeBlobArray(BlobArray& blob)
	{
		writeUInt(blob.getCount());

		if (blob.getCount() > 0)
			write(reinterpret_cast<char*>(blob.getIndexPtr(0)), blob.getArraySize());
	}

	std::string RawPacket::readString()
	{
		uint32_t nLen = readUInt();
		if (!m_bValidRead)
			return "";

		uint8_t* pBuffer = new uint8_t[nLen + 1]; //Add space for string terminator
		if (!read(reinterpret_cast<char*>(pBuffer), sizeof(pBuffer)))
			return "";

		pBuffer[nLen] = 0;

		return std::string(reinterpret_cast<char*>(pBuffer));
	}

	bool RawPacket::readBlobArray(BlobArray& blob)
	{
		if (!m_bValidRead)
			return false;

		if (!m_pData || blob.getCount() != 0) // Can't read without data or in an already filled blob
		{
			m_bValidRead = false;
			return false;
		}

		uint32_t nBlobCount = readUInt();
		if (!m_bValidRead)
			return false;

		// Can't read more data than the packet buffer contains
		if (m_nRead + (nBlobCount * blob.getBlobSize()) > m_nSize)
		{
			m_bValidRead = false;
			return false;
		}

		if (nBlobCount > 0)
			blob.add(m_pData + m_nRead, nBlobCount);

		m_nRead += nBlobCount * blob.getBlobSize();

		return true;
	}

	bool RawPacket::_onReceive(uint8_t* pData, uint32_t nLength)
	{
		// Can't read into an already filled packet
		if (m_pData)
			return false;

		m_pData = new uint8_t[nLength];
		m_nSize = nLength;

		memcpy(m_pData, pData, nLength);

		m_bValidRead = true; //Allow to read from the packet

		return true;
	}
}
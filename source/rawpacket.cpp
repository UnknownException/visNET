#include "visnet.h"
#include "rawpacket.h"

namespace visNET{
	RawPacket::RawPacket()
	{
		m_nSize = 0;
		m_pData = nullptr;

		//Not used when building a packet...
		m_nRead = 0;
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

	void RawPacket::read(char* pBuffer, uint32_t nSize)
	{
		if (!m_pData)
			throw std::exception("Cannot read from an uninitialized packet");

		if (m_nRead + nSize > m_nSize)
			throw std::exception("Cannot read more data than the buffer contains");

		memcpy(pBuffer, m_pData + m_nRead, nSize);

		m_nRead += nSize;
	}

	void RawPacket::readSkip(uint32_t nOffset)
	{
		if (!m_pData)
			throw std::exception("Cannot read from an uninitialized packet");

		if (m_nRead + nOffset > m_nSize)
			throw std::exception("Cannot read more data than the buffer contains");

		m_nRead += nOffset;
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
			write(reinterpret_cast<char*>(blob.read(0)), blob.getTotalSize());
	}

	std::string RawPacket::readString()
	{
		uint32_t nLen = readUInt();
		uint8_t* pBuffer = new uint8_t[nLen + 1]; //Add space for string terminator

		read(reinterpret_cast<char*>(pBuffer), sizeof(pBuffer));
		pBuffer[nLen] = 0;

		return std::string(reinterpret_cast<char*>(pBuffer));
	}

	void RawPacket::readBlobArray(BlobArray& blob)
	{
		if (!m_pData)
			throw std::exception("Cannot read from an uninitialized packet");

		if (blob.getCount() != 0)
			throw std::exception("Trying to read into an already filled blob");

		uint32_t nBlobCount = readUInt();

		if (m_nRead + (nBlobCount * blob.getSize()) > m_nSize)
			throw std::exception("Cannot read more data than the buffer contains");

		if (nBlobCount > 0)
			blob.add(m_pData + m_nRead, nBlobCount);

		m_nRead += nBlobCount * blob.getSize();
	}

	void RawPacket::onReceive(uint8_t* pData, uint32_t nLength)
	{
		if (m_pData)
		{
			delete[] m_pData;
			m_nRead = 0;
		}

		m_pData = new uint8_t[nLength];
		m_nSize = nLength;

		memcpy(m_pData, pData, nLength);
	}
}
#include "visnet.h"
#include "packet.h"

namespace visNET{
	Packet::Packet()
	{
		m_nSize = 0;
		m_pData = nullptr;

		m_nCursor = 0;
		m_eState = PS_WRITABLE;

		writeUInt32(0); // First 4 bytes are reserved for the packets size
	}

	Packet::~Packet()
	{
		if (m_pData)
			delete[] m_pData;
	}

	void Packet::_write(const uint8_t* pData, uint32_t nLength)
	{
		if (!isState(PS_WRITABLE))
			return;

		if (!m_pData)
			m_pData = new uint8_t[nLength];
		else
			m_pData = (uint8_t*)realloc(m_pData, m_nSize + nLength);

		memcpy(m_pData + m_nSize, pData, nLength);
		m_nSize += nLength;

		if (m_nSize > _visNET_PACKETSIZE_LIMIT)
			setState(PS_INVALID);
	}

	void Packet::writeString(const char* str)
	{
		if (!isState(PS_WRITABLE))
			return;

		uint32_t nLen = strlen(str);
		
		writeUInt32(nLen);
		write(str, nLen);
	}

	bool Packet::_read(uint8_t* pBuffer, uint32_t nSize)
	{
		if (!isState(PS_READABLE))
			return false;

		if (!m_pData || m_nCursor + nSize > m_nSize) // Can't read without data or outside bounds
		{
			setState(PS_INVALID);
			return false;
		}

		memcpy(pBuffer, m_pData + m_nCursor, nSize);

		m_nCursor += nSize;

		return true;
	}

	bool Packet::readSkip(uint32_t nOffset)
	{
		if (!isState(PS_READABLE))
			return false;

		if (!m_pData || m_nCursor + nOffset > m_nSize) // Can't read without data or outside of bounds
		{
			setState(PS_INVALID);
			return false;
		}

		m_nCursor += nOffset;

		return true;
	}

	std::string Packet::readString()
	{
		uint32_t nLen = readUInt32();
		if (!isState(PS_READABLE))
			return "";

		uint8_t* pBuffer = new uint8_t[nLen + 1]; //Add space for string terminator
		if (!read(pBuffer, nLen))
			return "";

		pBuffer[nLen] = 0;

		std::string result(reinterpret_cast<char*>(pBuffer));

		delete[] pBuffer;

		return result;
	}

	int32_t Packet::_onReceive(uint8_t* pData, uint32_t nLength)
	{
		// Can't read into an already filled packet, except if its in receive state
		if (!isState(PS_WRITABLE) && !isState(PS_INRECEIVE))
			return -1;

		if (isState(PS_WRITABLE))
		{
			delete[] m_pData;
			m_pData = new uint8_t[sizeof(m_nSize)];
			m_nSize = 0;

			setState(PS_INRECEIVE);
		}

		// If we haven't retrieved the packetsize yet
		uint32_t nRead = 0;
		if (m_nSize == 0)
		{
			// Only copy enough to get the full packet size
			uint32_t nDiff = sizeof(m_nSize) - m_nCursor;
			nRead = nLength > nDiff ? nDiff : nLength;

			memcpy(m_pData + m_nCursor, pData, nRead);
			m_nCursor += nRead;
			nLength -= nRead;

			if (m_nCursor < sizeof(m_nSize)) // Haven't received all size bytes yet
				return 0;
			else
			{
				memcpy(&m_nSize, m_pData, sizeof(m_nSize)); // Get real size
				if (m_nSize > _visNET_PACKETSIZE_LIMIT || m_nSize < sizeof(m_nSize))
				{
					setState(PS_INVALID);
					return -1;
				}

				m_pData = (uint8_t*)realloc(m_pData, m_nSize);
			}
		}

		uint32_t nAdditionalData = 0;

		// Only copy when we actually have data to write
		if (nLength != 0)
		{
			// Do not read more than required, cut off!
			if (nLength + m_nCursor <= m_nSize)
			{
				memcpy(m_pData + m_nCursor, pData + nRead, nLength);
				m_nCursor += nLength;
			}
			else
			{
				nAdditionalData = (nLength + m_nCursor) - m_nSize;
				memcpy(m_pData + m_nCursor, pData + nRead, m_nSize - m_nCursor);
				m_nCursor = m_nSize;
			}
		}

		if (m_nSize == m_nCursor)
		{
			m_nCursor = sizeof(m_nSize); // Set the cursor back for reading (0 + 4)
			setState(PS_READABLE);
		}

		return nAdditionalData; // 0 if no additionals
	}

	bool Packet::_onSend() 
	{
		if (!isWritable())
			return false;

		memcpy(m_pData, &m_nSize, sizeof(m_nSize)); // Overwrite first 4 bytes

		return true;
	}

	std::shared_ptr<Packet> Packet::_copy()
	{
		if (!isWritable())
			return nullptr;

		std::shared_ptr<Packet> copy = std::make_shared<Packet>();
		// Delete initial 4 bytes
		if (copy->m_pData)
		{
			delete[] copy->m_pData;
			copy->m_pData = nullptr;
			copy->m_nSize = 0;
		}
		copy->write(m_pData, m_nSize);

		return copy;
	}
}
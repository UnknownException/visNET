#include "visnet.h"
#include "packet.h"

namespace visNET{
	Packet::Packet()
	{
		m_Cursor = 0;
		m_PacketState = PS_CLEAR;
	}

	Packet::~Packet()
	{
	}

	void Packet::_write(const uint8_t* data, uint32_t dataSize)
	{
		if (isState(PS_CLEAR))
		{
			setState(PS_WRITABLE);
			writeUInt32(0); // First 4 bytes are reserved for the packets size
		}

		if (!isState(PS_WRITABLE))
			return;

		m_Data.resize(m_Data.size() + dataSize);
		memcpy(&m_Data[m_Data.size() - dataSize], data, dataSize);

		if (m_Data.size() > _visNET_PACKETSIZE_LIMIT)
			setState(PS_INVALID);
	}

	void Packet::writeString(const char* string)
	{
		if (!isState(PS_WRITABLE) && !isState(PS_CLEAR))
			return;

		uint32_t stringLength = strlen(string);
		writeUInt32(stringLength);
		write(string, stringLength);
	}

	bool Packet::_read(uint8_t* buffer, uint32_t bufferSize)
	{
		if (!isState(PS_READABLE))
			return false;

		if (m_Cursor + bufferSize > m_Data.size()) // Can't read outside of bounds
		{
			setState(PS_INVALID);
			return false;
		}

		memcpy(buffer, &m_Data[m_Cursor], bufferSize);
		m_Cursor += bufferSize;

		return true;
	}

	bool Packet::readSkip(uint32_t skipSize)
	{
		if (!isState(PS_READABLE))
			return false;

		if (m_Cursor + skipSize > m_Data.size()) // Can't read outside of bounds
		{
			setState(PS_INVALID);
			return false;
		}

		m_Cursor += skipSize;

		return true;
	}

	std::string Packet::readString()
	{
		uint32_t stringSize = readUInt32();
		if (!isState(PS_READABLE))
			return "";

		uint8_t* stringBuffer = new uint8_t[stringSize + 1]; //Add space for string terminator
		if (!read(stringBuffer, stringSize))
		{
			delete[] stringBuffer;
			return "";
		}

		stringBuffer[stringSize] = 0;

		std::string result(reinterpret_cast<char*>(stringBuffer));

		delete[] stringBuffer;

		return result;
	}

	uint32_t Packet::_onReceive(uint8_t* data, uint32_t dataSize)
	{
		if (isState(PS_CLEAR))
			setState(PS_INRECEIVE);
		else if (!isState(PS_INRECEIVE))
			return 0;

		uint32_t dataRead = 0;

		if (m_Cursor < PacketSizeLength)
		{
			uint32_t difference = PacketSizeLength - m_Cursor;
			if (dataSize < difference)
			{
				m_Data.resize(m_Data.size() + dataSize);
				memcpy(&m_Data[m_Cursor], data, dataSize);
				m_Cursor += dataSize;

				return 0; // No remaining bytes
			}

			m_Data.resize(m_Data.size() + difference);
			memcpy(&m_Data[m_Cursor], data, difference);
			m_Cursor += difference;
			dataRead += difference;

			uint32_t packetSize;
			memcpy(&packetSize, &m_Data[0], sizeof(PacketSizeLength));

			if (packetSize > _visNET_PACKETSIZE_LIMIT || packetSize < PacketSizeLength)
			{
				setState(PS_INVALID);
				return 0; // No remaining bytes
			}

			m_Data.resize(packetSize);
		}

		uint32_t remainingBytes = dataSize - dataRead;
		if (remainingBytes > 0)
		{
			uint32_t requiredBytes = m_Data.size() - m_Cursor;
			uint32_t readableBytes = remainingBytes < requiredBytes ? remainingBytes : requiredBytes;
			memcpy(&m_Data[m_Cursor], data + dataRead, readableBytes);
			m_Cursor += readableBytes;
			remainingBytes -= readableBytes;
		}

		if (m_Data.size() == m_Cursor)
		{
			m_Cursor = PacketSizeLength; // Set the cursor after packet size
			setState(PS_READABLE);
		}

		return remainingBytes;
	}

	bool Packet::_onSend() 
	{
		if (!isState(PS_WRITABLE))
			return false;

		uint32_t packetSize = m_Data.size();
		memcpy(&m_Data[0], &packetSize, sizeof(packetSize)); // Overwrite first 4 bytes

		return true;
	}
	
	std::shared_ptr<Packet> Packet::_copy()
	{
		std::shared_ptr<Packet> copy = std::make_shared<Packet>();
		copy->m_Data = m_Data;
		copy->m_PacketState = m_PacketState;
		copy->m_Cursor = m_Cursor;

		return copy;
	}
}
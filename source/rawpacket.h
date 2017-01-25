#pragma once

namespace visNET{
	class RawPacket{
		uint32_t m_nSize;
		uint8_t* m_pData;

		uint32_t m_nRead;
		bool m_bValidRead;
	public:
		RawPacket();
		virtual ~RawPacket();

		// If possible, use the predefined writetypes instead of directly writing to the buffer
		void write(const char* data, uint32_t length);

		void writeInt(int32_t n) { write(reinterpret_cast<char*>(&n), sizeof(int32_t)); }
		void writeUInt(uint32_t n) { write(reinterpret_cast<char*>(&n), sizeof(uint32_t)); }
		void writeShort(int16_t n) { write(reinterpret_cast<char*>(&n), sizeof(int16_t)); }
		void writeUShort(uint16_t n) { write(reinterpret_cast<char*>(&n), sizeof(uint16_t)); }
		void writeChar(int8_t n) { write(reinterpret_cast<char*>(&n), sizeof(int8_t)); }
		void writeUChar(uint8_t n) { write(reinterpret_cast<char*>(&n), sizeof(uint8_t)); }
		void writeFloat(float_t f) { write(reinterpret_cast<char*>(&f), sizeof(float_t)); }
		void writeDouble(double_t d) { write(reinterpret_cast<char*>(&d), sizeof(double_t)); }
		void writeBool(bool b) { write(reinterpret_cast<char*>(&b), sizeof(bool)); }

		void writeString(const char* str);
		void writeString(std::string str) { writeString(str.c_str()); } //Alias
		void writeBlobArray(BlobArray& blob); // Efficient way to send multiple objects

		// If possible, use the predefined readtypes instead of directly reading from the buffer
		bool read(char* buffer, uint32_t size);
		// Skip an amount of bytes in the received packet
		bool readSkip(uint32_t offset);

		int32_t readInt(){
			int32_t n = 0;
			read(reinterpret_cast<char*>(&n), sizeof(int32_t));
			return n;
		}
		uint32_t readUInt(){
			uint32_t n = 0;
			read(reinterpret_cast<char*>(&n), sizeof(uint32_t));
			return n;
		}

		int16_t readShort(){
			int16_t n = 0;
			read(reinterpret_cast<char*>(&n), sizeof(int16_t));
			return n;
		}

		uint16_t readUShort(){
			uint16_t n = 0;
			read(reinterpret_cast<char*>(&n), sizeof(int16_t));
			return n;
		}

		int8_t readChar(){
			int8_t n = 0;
			read(reinterpret_cast<char*>(&n), sizeof(int8_t));
			return n;
		}

		uint8_t readUChar() {
			uint8_t n = 0;
			read(reinterpret_cast<char*>(&n), sizeof(uint8_t));
			return n;
		}

		float_t readFloat() {
			float_t f = 0.f;
			read(reinterpret_cast<char*>(&f), sizeof(float_t));
			return f;
		}

		double_t readDouble() {
			double_t d = 0.f;
			read(reinterpret_cast<char*>(&d), sizeof(double_t));	
			return d;
		}

		bool readBool(){
			bool b = false;
			read(reinterpret_cast<char*>(&b), sizeof(bool));
			return b;
		}

		std::string readString();
		bool readBlobArray(BlobArray& blob);

		bool readCorrectly() { return m_bValidRead; }

		// Put these near the bottom in intellisense
		// These _* functions shouldn't get called from any extern program except if declaring a custom packet type
		const uint8_t* _getRawData() { return m_pData; }
		const uint32_t _getRawSize() { return m_nSize; }

		virtual bool _onReceive(uint8_t* pData, uint32_t nLength); //Gets called after receiving
		virtual void _onSend() {}; //Gets called before being send
	};
}
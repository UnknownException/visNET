#pragma once

namespace visNET{
	class Packet{
		uint32_t m_nSize;
		uint8_t* m_pData;

		uint32_t m_nCursor;

		enum PSTATES{
			PS_INVALID,
			PS_WRITABLE,
			PS_INRECEIVE,
			PS_READABLE,
		} m_eState;

		void setState(PSTATES e) { m_eState = e; }
		bool isState(PSTATES e) { return m_eState == e; }
	public:
		Packet();
		virtual ~Packet();

		// If possible, use the predefined writetypes instead of directly writing to the buffer
	private:
		void _write(const uint8_t* data, uint32_t length);
	public:
		template<typename T>
		void write(const T* data, uint32_t length){
			_write(reinterpret_cast<const uint8_t*>(data), length);
		}

		void writeInt(int32_t n) { write(&n, sizeof(int32_t)); }
		void writeUInt(uint32_t n) { write(&n, sizeof(uint32_t)); }
		void writeShort(int16_t n) { write(&n, sizeof(int16_t)); }
		void writeUShort(uint16_t n) { write(&n, sizeof(uint16_t)); }
		void writeChar(int8_t n) { write(&n, sizeof(int8_t)); }
		void writeUChar(uint8_t n) { write(&n, sizeof(uint8_t)); }
		void writeFloat(float_t f) { write(&f, sizeof(float_t)); }
		void writeDouble(double_t d) { write(&d, sizeof(double_t)); }
		void writeBool(bool b) { write(&b, sizeof(bool)); }

		void writeString(const char* str);
#ifndef _visNET_EXCLUDE_BLOBARRAY
		void writeBlobArray(BlobArray& blob); // Efficient way to send multiple objects
#endif

		// If possible, use the predefined readtypes instead of directly reading from the buffer
	private:
		bool _read(uint8_t* buffer, uint32_t size);
	public:
		template<typename T>
		bool read(T* buffer, uint32_t size){
			return _read(reinterpret_cast<uint8_t*>(buffer), size);
		}

		// Skip an amount of bytes in the received packet
		bool readSkip(uint32_t offset);

		int32_t readInt(){
			int32_t n = 0;
			read(&n, sizeof(int32_t));
			return n;
		}
		uint32_t readUInt(){
			uint32_t n = 0;
			read(&n, sizeof(uint32_t));
			return n;
		}

		int16_t readShort(){
			int16_t n = 0;
			read(&n, sizeof(int16_t));
			return n;
		}

		uint16_t readUShort(){
			uint16_t n = 0;
			read(&n, sizeof(int16_t));
			return n;
		}

		int8_t readChar(){
			int8_t n = 0;
			read(&n, sizeof(int8_t));
			return n;
		}

		uint8_t readUChar() {
			uint8_t n = 0;
			read(&n, sizeof(uint8_t));
			return n;
		}

		float_t readFloat() {
			float_t f = 0.f;
			read(&f, sizeof(float_t));
			return f;
		}

		double_t readDouble() {
			double_t d = 0.f;
			read(&d, sizeof(double_t));	
			return d;
		}

		bool readBool(){
			bool b = false;
			read(&b, sizeof(bool));
			return b;
		}

		std::string readString();

#ifndef _visNET_EXCLUDE_BLOBARRAY
		bool readBlobArray(BlobArray& blob);
#endif

		bool isReadable() { return isState(PS_READABLE); } // When the state is not readable, it hasn't read correctly
		bool isValid() { return !isState(PS_INVALID); }
		bool isWritable() { return isState(PS_WRITABLE); }
		bool isTransfering() { return isState(PS_INRECEIVE); }

		// Put these near the bottom in intellisense
		// These _* functions shouldn't get called from any extern program except if declaring a custom packet type
		const uint8_t* _getRawData() { return m_pData; }
		const uint32_t _getRawSize() { return m_nSize; }

		// onReceive can return multiple values, two values have a special meaning
		// -1: Packet is incorrect
		// 0: Packet is correct
		// >0: Packet is finished, we have additional data (new packet)

		int32_t _onReceive(uint8_t* pData, uint32_t nLength); 
		bool _onSend(); //Gets called before being send

		void _swap(Packet* packet) {
			//Save current ones
			decltype(m_pData) pOldData = m_pData;
			decltype(m_nSize) nOldSize = m_nSize;
			decltype(m_eState) eOldState = m_eState;
			decltype(m_nCursor) nOldCursor = m_nCursor;

			//Transfer values
			m_pData = packet->m_pData;
			m_nSize = packet->m_nSize;
			m_eState = packet->m_eState;
			m_nCursor = packet->m_nCursor;

			packet->m_pData = pOldData;
			packet->m_nSize = nOldSize;
			packet->m_eState = eOldState;
			packet->m_nCursor = nOldCursor;
		}
	};
}
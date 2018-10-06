#pragma once

namespace visNET{
	class __declspec(dllexport) Packet {
		uint32_t m_nSize;
		uint8_t* m_pData;

		uint32_t m_nCursor;

		enum PSTATES {
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
		void write(const T* data, uint32_t length) {
			_write(reinterpret_cast<const uint8_t*>(data), length);
		}

		void writeInt8(int8_t n) { write(&n, sizeof(int8_t)); }
		void writeInt16(int16_t n) { write(&n, sizeof(int16_t)); }
		void writeInt32(int32_t n) { write(&n, sizeof(int32_t)); }
		void writeInt64(int64_t n) { write(&n, sizeof(int64_t)); }
		void writeUInt8(uint8_t n) { write(&n, sizeof(uint8_t)); }
		void writeUInt16(uint16_t n) { write(&n, sizeof(uint16_t)); }
		void writeUInt32(uint32_t n) { write(&n, sizeof(uint32_t)); }
		void writeUInt64(uint64_t n) { write(&n, sizeof(uint64_t)); }
		void writeFloat(float f) { write(&f, sizeof(float)); }
		void writeDouble(double d) { write(&d, sizeof(double)); }
		void writeBool(bool b) { write(&b, sizeof(bool)); }

		void writeString(const char* str);
		void writeString(std::string& str) { writeString(str.c_str()); }

		template <typename T>
		void writeBlobArray(BlobArray<T>& blob)
		{
			if (!isState(PS_WRITABLE))
				return;

			writeUInt32(blob.getItemCount());

			if (blob.getItemCount() > 0)
				write(blob.get(0), blob.getSize());
		}

		// If possible, use the predefined readtypes instead of directly reading from the buffer
	private:
		bool _read(uint8_t* buffer, uint32_t size);
	public:
		template<typename T>
		bool read(T* buffer, uint32_t size) {
			return _read(reinterpret_cast<uint8_t*>(buffer), size);
		}

		// Skip an amount of bytes in the received packet
		bool readSkip(uint32_t offset);

		int8_t readInt8() {
			int8_t n = 0;
			read(&n, sizeof(int8_t));
			return n;
		}

		int16_t readInt16() {
			int16_t n = 0;
			read(&n, sizeof(int16_t));
			return n;
		}

		int32_t readInt32() {
			int32_t n = 0;
			read(&n, sizeof(int32_t));
			return n;
		}

		int64_t readInt64() {
			int64_t n = 0;
			read(&n, sizeof(int64_t));
			return n;
		}

		uint8_t readUInt8() {
			uint8_t n = 0;
			read(&n, sizeof(uint8_t));
			return n;
		}

		uint16_t readUInt16() {
			uint16_t n = 0;
			read(&n, sizeof(int16_t));
			return n;
		}

		uint32_t readUInt32() {
			uint32_t n = 0;
			read(&n, sizeof(uint32_t));
			return n;
		}

		uint64_t readUInt64() {
			uint64_t n = 0;
			read(&n, sizeof(uint64_t));
			return n;
		}

		float readFloat() {
			float f = 0.f;
			read(&f, sizeof(float));
			return f;
		}

		double readDouble() {
			double d = 0.f;
			read(&d, sizeof(double));
			return d;
		}

		bool readBool() {
			bool b = false;
			read(&b, sizeof(bool));
			return b;
		}

		std::string readString();

		template <typename T>
		std::shared_ptr<BlobArray<T>> readBlobArray(){
			auto blob = std::make_shared<BlobArray<T>>();
			if (!isState(PS_READABLE))
				return blob;

			if (!m_pData || blob->getItemCount() != 0) // Can't read without data or in an already filled blob
			{
				setState(PS_INVALID);
				return blob;
			}

			uint32_t nItemCount = readUInt32();
			if (!isState(PS_READABLE))
				return blob;

			// Can't read more data than the packet buffer contains
			if (m_nCursor + (nItemCount * blob->getItemSize()) > m_nSize)
			{
				setState(PS_INVALID);
				return blob;
			}

			if (nItemCount > 0)
				blob->add((T*)(m_pData + m_nCursor), nItemCount);

			m_nCursor += nItemCount * blob->getItemSize();

			return blob;
		}

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
		std::shared_ptr<Packet> _copy();
	};
}
#pragma once

namespace visNET{
	class DLLEXPORT Packet {
		const uint32_t PacketSizeLength = sizeof(uint32_t);

		std::vector<uint8_t> m_Data;
		uint32_t m_Cursor;

		enum PSTATES {
			PS_INVALID,
			PS_CLEAR,
			PS_WRITABLE,
			PS_INRECEIVE,
			PS_READABLE,
		} m_PacketState;

		void setState(PSTATES packetState) { m_PacketState = packetState; }
		bool isState(PSTATES packetState) { return m_PacketState == packetState; }
	public:
		Packet();
		virtual ~Packet();

	private:
		void _write(const uint8_t* data, uint32_t dataSize);
	public:
		// If possible, use the predefined functions instead of directly writing to the buffer
		template<typename T>
		void write(const T* data, uint32_t dataSize) {
			_write(reinterpret_cast<const uint8_t*>(data), dataSize);
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

		void writeString(const char* string);
		void writeString(std::string& string) { writeString(string.c_str()); }

		template <typename T>
		void writeBlobArray(BlobArray<T>& blob)
		{
			if (!isState(PS_WRITABLE) && !isState(PS_CLEAR))
				return;

			writeUInt32(blob.size());

			if (blob.size() > 0)
				write(blob.getPtr(0), blob.dataSize());
		}

		// If possible, use the predefined readtypes instead of directly reading from the buffer
	private:
		bool _read(uint8_t* buffer, uint32_t bufferSize);
	public:
		template<typename T>
		bool read(T* buffer, uint32_t bufferSize) {
			return _read(reinterpret_cast<uint8_t*>(buffer), bufferSize);
		}

		// Skip an amount of bytes in the received packet
		bool readSkip(uint32_t skipSize);

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
				return nullptr;

			uint32_t itemCount = readUInt32();
			if (!isState(PS_READABLE))
				return nullptr;

			// Prevent reading out of bounds
			if (m_Cursor + (itemCount * sizeof(T)) > m_Data.size())
			{
				setState(PS_INVALID);
				return nullptr;
			}

			if (itemCount > 0)
				blob->add((T*)(&m_Data[m_Cursor]), itemCount);

			m_Cursor += itemCount * sizeof(T);

			return blob;
		}

		bool isReadable() { return isState(PS_READABLE); }
		bool isValid() { return !isState(PS_INVALID); }
		bool isWritable() { return isState(PS_WRITABLE); }
		bool isTransfering() { return isState(PS_INRECEIVE); }

		// Put these near the bottom in intellisense
		// These _* functions shouldn't get called from any extern program except if declaring a custom packet type
		const uint8_t* _getRawData() { return &m_Data[0]; }
		const uint32_t _getRawSize() { return m_Data.size(); }

		// Return value of onReceive indicates if there is unread data remaining in the given buffer
		uint32_t _onReceive(uint8_t* data, uint32_t dataSize);
		bool _onSend(); //Gets called before being send
		std::shared_ptr<Packet> _copy();
	};
}
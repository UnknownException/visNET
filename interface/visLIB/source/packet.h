#pragma once

extern "C" {
	__declspec(dllexport) void* PacketCreate();
	__declspec(dllexport) void PacketDestroy(void* instance);

	/* Write functions */
	__declspec(dllexport) void PacketWriteInt8(void* instance, int8_t n);
	__declspec(dllexport) void PacketWriteInt16(void* instance, int16_t n);
	__declspec(dllexport) void PacketWriteInt32(void* instance, int32_t n);
	__declspec(dllexport) void PacketWriteInt64(void* instance, int64_t n);

	__declspec(dllexport) void PacketWriteUInt8(void* instance, uint8_t n);
	__declspec(dllexport) void PacketWriteUInt16(void* instance, uint16_t n);
	__declspec(dllexport) void PacketWriteUInt32(void* instance, uint32_t n);
	__declspec(dllexport) void PacketWriteUInt64(void* instance, uint64_t n);

	__declspec(dllexport) void PacketWriteFloat(void* instance, float f);
	__declspec(dllexport) void PacketWriteDouble(void* instance, double d);

	__declspec(dllexport) void PacketWriteBool(void* instance, bool b);

	/*
	void write(buffer, amount);
	void writeString(const char* str);
	// Blob arrays
	*/

	/* Read functions */
	__declspec(dllexport) int8_t PacketReadInt8(void* instance);
	__declspec(dllexport) int16_t PacketReadInt16(void* instance);
	__declspec(dllexport) int32_t PacketReadInt32(void* instance);
	__declspec(dllexport) int64_t PacketReadInt64(void* instance);

	__declspec(dllexport) uint8_t PacketReadUInt8(void* instance);
	__declspec(dllexport) uint16_t PacketReadUInt16(void* instance);
	__declspec(dllexport) uint32_t PacketReadUInt32(void* instance);
	__declspec(dllexport) uint64_t PacketReadUInt64(void* instance);

	__declspec(dllexport) float PacketReadFloat(void* instance);
	__declspec(dllexport) double PacketReadDouble(void* instance);

	__declspec(dllexport) bool PacketReadBool(void* instance);

	/*
	int8_t* read(amount);
	std::string readString();
	// Blob arrays
	readSkip
	*/

	/* Flags */
	__declspec(dllexport) bool PacketIsReadable(void* instance);
	__declspec(dllexport) bool PacketIsValid(void* instance);
	__declspec(dllexport) bool PacketIsWritable(void* instance);
}
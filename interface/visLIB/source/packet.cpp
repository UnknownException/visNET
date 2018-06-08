#include "vislib.h"
#include "packet.h"
#include "../../../source/visnet.h"

extern "C"
{
	__declspec(dllexport) void* PacketCreate()
	{
		return (void*)new visNET::Packet();
	}

	__declspec(dllexport) void PacketDestroy(void* instance)
	{
		delete (visNET::Packet*)instance;
	}

/* 
	Write functions 
*/

	__declspec(dllexport) void PacketWriteInt8(void* instance, int8_t n)
	{
		((visNET::Packet*)instance)->writeInt8(n);
	}

	__declspec(dllexport) void PacketWriteInt16(void* instance, int16_t n)
	{
		((visNET::Packet*)instance)->writeInt16(n);
	}

	__declspec(dllexport) void PacketWriteInt32(void* instance, int32_t n)
	{
		((visNET::Packet*)instance)->writeInt32(n);
	}

	__declspec(dllexport) void PacketWriteInt64(void* instance, int64_t n)
	{
		((visNET::Packet*)instance)->writeInt64(n);
	}

	__declspec(dllexport) void PacketWriteUInt8(void* instance, uint8_t n)
	{
		((visNET::Packet*)instance)->writeUInt8(n);
	}

	__declspec(dllexport) void PacketWriteUInt16(void* instance, uint16_t n)
	{
		((visNET::Packet*)instance)->writeUInt16(n);
	}

	__declspec(dllexport) void PacketWriteUInt32(void* instance, uint32_t n)
	{
		((visNET::Packet*)instance)->writeUInt32(n);
	}

	__declspec(dllexport) void PacketWriteUInt64(void* instance, uint64_t n)
	{
		((visNET::Packet*)instance)->writeUInt64(n);
	}

	__declspec(dllexport) void PacketWriteFloat(void* instance, float f)
	{
		((visNET::Packet*)instance)->writeFloat(f);
	}

	__declspec(dllexport) void PacketWriteDouble(void* instance, double d)
	{
		((visNET::Packet*)instance)->writeDouble(d);
	}

	__declspec(dllexport) void PacketWriteBool(void* instance, bool b)
	{
		((visNET::Packet*)instance)->writeBool(b);
	}

/*
	Read functions
*/

	__declspec(dllexport) int8_t PacketReadInt8(void* instance)
	{
		return ((visNET::Packet*)instance)->readInt8();
	}

	__declspec(dllexport) int16_t PacketReadInt16(void* instance)
	{
		return ((visNET::Packet*)instance)->readInt16();
	}

	__declspec(dllexport) int32_t PacketReadInt32(void* instance)
	{
		return ((visNET::Packet*)instance)->readInt32();
	}

	__declspec(dllexport) int64_t PacketReadInt64(void* instance)
	{
		return ((visNET::Packet*)instance)->readInt64();
	}

	__declspec(dllexport) uint8_t PacketReadUInt8(void* instance)
	{
		return ((visNET::Packet*)instance)->readUInt8();
	}

	__declspec(dllexport) uint16_t PacketReadUInt16(void* instance)
	{
		return ((visNET::Packet*)instance)->readUInt16();
	}

	__declspec(dllexport) uint32_t PacketReadUInt32(void* instance)
	{
		return ((visNET::Packet*)instance)->readUInt32();
	}

	__declspec(dllexport) uint64_t PacketReadUInt64(void* instance)
	{
		return ((visNET::Packet*)instance)->readUInt64();
	}

	__declspec(dllexport) float PacketReadFloat(void* instance)
	{
		return ((visNET::Packet*)instance)->readFloat();
	}

	__declspec(dllexport) double PacketReadDouble(void* instance)
	{
		return ((visNET::Packet*)instance)->readDouble();
	}

	__declspec(dllexport) bool PacketReadBool(void* instance)
	{
		return ((visNET::Packet*)instance)->readBool();
	}

/* 
	Flags
*/

	__declspec(dllexport) bool PacketIsReadable(void* instance)
	{
		return ((visNET::Packet*)instance)->isReadable();
	}

	__declspec(dllexport) bool PacketIsValid(void* instance)
	{
		return ((visNET::Packet*)instance)->isValid();
	}

	__declspec(dllexport) bool PacketIsWritable(void* instance)
	{
		return ((visNET::Packet*)instance)->isWritable();
	}
};

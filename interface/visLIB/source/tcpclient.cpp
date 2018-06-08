#include "vislib.h"
#include "tcpclient.h"
#include "../../../source/visnet.h"

#include <Windows.h>

extern "C" 
{
	__declspec(dllexport) void* TcpClientCreate(char* ip, uint16_t port)
	{
		return (void*)new visNET::TcpClient(ip, port);
	}

	__declspec(dllexport) void TcpClientDestroy(void* instance)
	{
		delete (visNET::TcpClient*)instance;
	}

	__declspec(dllexport) void TcpClientSend(void* instance, void* packet)
	{
		((visNET::TcpClient*)instance)->send(*(visNET::Packet*)packet);
	}

	__declspec(dllexport) void* TcpClientReceive(void* instance)
	{
		return nullptr;
		//	return ((visNET::TcpClient*)instance)->receive();
	}

	__declspec(dllexport) bool TcpClientIsDisconnected(void* instance)
	{
		return ((visNET::TcpClient*)instance)->isDisconnected();
	}

	__declspec(dllexport) bool TcpClientIsValid(void* instance)
	{
		return ((visNET::TcpClient*)instance)->isValid();
	}

	__declspec(dllexport) const char* TcpClientGetError(void* instance)
	{
		return ((visNET::TcpClient*)instance)->getError();
	}
};

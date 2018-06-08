#pragma once

extern "C" {
	__declspec(dllexport) void* TcpClientCreate(char* ip, uint16_t port);
	__declspec(dllexport) void TcpClientDestroy(void* instance);
	__declspec(dllexport) void TcpClientSend(void* instance, void* packet);
	__declspec(dllexport) void* TcpClientReceive(void* instance);
	__declspec(dllexport) bool TcpClientIsDisconnected(void* instance);
	__declspec(dllexport) bool TcpClientIsValid(void* instance);
	__declspec(dllexport) const char* TcpClientGetError(void* instance);
};
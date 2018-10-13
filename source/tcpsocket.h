#pragma once

namespace visNET {
	class TcpSocket : public ISocket {
	public:
		TcpSocket();
		virtual ~TcpSocket();

		virtual bool listenOn(uint16_t nPort);
		virtual bool connectTo(const char* pszIp, uint16_t nPort);

		virtual bool setNonBlocking(bool b);

		virtual bool write(const uint8_t* pBuffer, int32_t nSize);
		virtual int32_t read(uint8_t* pBuffer, int32_t nSize);
	};
}
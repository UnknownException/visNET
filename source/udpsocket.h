#pragma once

namespace visNET {
	class UdpSocket : public ISocket {
	public:
		UdpSocket();
		virtual ~UdpSocket();

		virtual bool bindToPort(uint16_t nPort);

		virtual bool setNonBlocking(bool b);

		virtual bool write(const uint8_t* pBuffer, int32_t nSize, const char* szIP, const uint16_t nPort);
		virtual std::pair<sockaddr_in, int32_t> read(uint8_t* pBuffer, int32_t nSize);
	};
}
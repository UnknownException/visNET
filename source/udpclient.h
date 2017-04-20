#pragma once
#include "networkobject.h"

namespace visNET{
	class UdpClient : public NetworkObject{
	public:
		UdpClient(uint16_t nPort);
		virtual ~UdpClient();

		/*

		// Add ip and udp functions

		template <typename T>
		bool write(const T* buffer, int32_t size) { return getSocket()->write(buffer, size); }
		bool write(Packet& packet) { return getSocket()->write(packet); }

		template <typename T>
		int32_t read(T* buffer, int32_t size) { return getSocket()->read(buffer, size); }
		bool read(Packet& packet) { return getSocket()->read(packet); }
		*/
	};
}
#pragma once

namespace visNET {
	class UdpMessage {
	private:
		std::string m_ip;
		uint16_t m_port;
		std::shared_ptr<Packet> m_packet;
	public:
		UdpMessage(std::string& ip, uint16_t port, std::shared_ptr<Packet> packet) {
			m_ip = ip;
			m_port = port;
			m_packet = packet;
		}

		virtual ~UdpMessage() {

		}

		decltype(m_ip) getIP() { return m_ip; }
		decltype(m_port) getPort() { return m_port; }
		decltype(m_packet) getPacket() { return m_packet; }
	};
}
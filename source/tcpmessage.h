#pragma once

namespace visNETCore {
	class TcpMessage {
	private:
		ConnectionIdentifier m_connectionIdentifier;
		std::shared_ptr<Packet> m_packet;
	public:
		TcpMessage(ConnectionIdentifier identifier, std::shared_ptr<Packet> packet) {
			m_connectionIdentifier = identifier;
			m_packet = packet;
		}

		virtual ~TcpMessage() {

		}

		ConnectionIdentifier getConnectionIdentifier() { return m_connectionIdentifier; }
		std::shared_ptr<Packet> getPacket() { return m_packet; }
	};
}
#pragma once

namespace visNET{
	class IdentifiableSocket {
	private:
		ConnectionIdentifier m_socketIdentifier;
		std::shared_ptr<TcpSocket> m_socket;
	public:
		IdentifiableSocket(ConnectionIdentifier socketIdentifier, std::shared_ptr<TcpSocket> socket) {
			m_socketIdentifier = socketIdentifier;
			m_socket = socket;
		}

		virtual ~IdentifiableSocket() {

		}

		ConnectionIdentifier getConnectionIdentifier() { return m_socketIdentifier; }
		std::shared_ptr<TcpSocket> getSocket() { return m_socket; }
	};
}
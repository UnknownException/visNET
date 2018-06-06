#pragma once

namespace visNETCore {
	class IdentifiableSocket {
	private:
		ConnectionIdentifier m_socketIdentifier;
		std::shared_ptr<Socket> m_socket;
	public:
		IdentifiableSocket(ConnectionIdentifier socketIdentifier, std::shared_ptr<Socket> socket) {
			m_socketIdentifier = socketIdentifier;
			m_socket = socket;
		}

		virtual ~IdentifiableSocket() {

		}

		ConnectionIdentifier getConnectionIdentifier() { return m_socketIdentifier; }
		std::shared_ptr<Socket> getSocket() { return m_socket; }
	};
}
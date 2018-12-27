#pragma once

namespace visNET {
	class ISocket {
		std::size_t m_handle;
		bool m_bAlive;
	public:
		ISocket() { 
			m_handle = 0;
			m_bAlive = false; 
		}

		virtual ~ISocket() {
		}

		void setHandle(std::size_t s) { m_handle = s; }
		std::size_t getHandle() { return m_handle; }

		virtual bool setNonBlocking(bool b) = 0;

		void setAlive(bool b) { m_bAlive = b; }
		bool getAlive() { return m_bAlive; }
	};
}
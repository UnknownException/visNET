#pragma once

namespace visNET {
	class ISocket {
		unsigned long long m_handle;
		bool m_bAlive;
	public:
		ISocket() { 
			m_handle = 0;
			m_bAlive = false; 
		}

		virtual ~ISocket() {
		}

		void setHandle(unsigned long long s) { m_handle = s; }
		unsigned long long getHandle() { return m_handle; }

		virtual bool setNonBlocking(bool b) = 0;

		void setAlive(bool b) { m_bAlive = b; }
		bool getAlive() { return m_bAlive; }
	};
}
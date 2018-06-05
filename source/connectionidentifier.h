#pragma once

namespace visNETCore {
	class ConnectionIdentifier {
		uint32_t m_id;

	public:
		ConnectionIdentifier() {
			m_id = 0;
		}

		ConnectionIdentifier(uint32_t id) {
			m_id = id;
		}

		virtual ~ConnectionIdentifier() {

		}

		void increase() {
			++m_id;
		}

		bool isValid() {
			return m_id != 0;
		}

		decltype(m_id) getValue() {
			return m_id;
		}

		bool operator==(const ConnectionIdentifier& second)
		{
			return m_id == second.m_id;
		}
	};
}
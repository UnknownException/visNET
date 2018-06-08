#include "visnet.h"
#include "networkobject.h"

namespace visNET {
	NetworkObject::NetworkObject() 
	{ 
		m_socket = new Socket();
		m_strError = new std::string();
		m_bValid = false; 
	}

	NetworkObject::~NetworkObject()
	{
		delete m_socket;
		delete m_strError;
	}

	const std::string NetworkObject::getError()
	{
		return std::string(m_strError->c_str());
	}

	void NetworkObject::setError(const char* pszError) 
	{ 
		m_strError->clear();
		m_strError->append(pszError);
	}


	bool NetworkObject::setNonBlocking(bool b)
	{
		return getSocket()->setNonBlocking(b);
	}
}
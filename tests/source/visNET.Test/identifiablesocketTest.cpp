#include "pch.h"
#include <WinSock2.h>
#include "../../../source/socket.h"
#include "../../../source/identifiablesocket.h"

TEST(Structure_IdentifiableSocket, CreateIdentifiableSocket)
{
	// Assign
	visNET::ConnectionIdentifier id(123);

	// Act
	visNET::IdentifiableSocket sock(id, nullptr);

	// Assert
	EXPECT_EQ(123, sock.getConnectionIdentifier().getValue());
	EXPECT_EQ(nullptr, sock.getSocket());
}
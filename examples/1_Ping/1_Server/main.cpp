#include "../../base.h"

int main()
{
	visNET::startup();

	visNET::TcpListener server(8901);
	if (!server.isValid())
	{
		std::cout << "Failed to create listener on port 8901" << std::endl;

		// Prevents the console to close
		waitForInput();

		return -1;
	}

	std::cout << "Waiting for a client to connect" << std::endl;

	auto connection = server.getConnection();
	while (!connection.isValid())
	{
		std::this_thread::sleep_for(std::chrono::microseconds(100));

		connection = server.getConnection();
	}

	std::cout << "Client connected" << std::endl;

	auto receive = server.receive();
	while (receive.empty())
	{
		std::this_thread::sleep_for(std::chrono::microseconds(100));

		receive = server.receive();
	}

	std::cout << "Received message from client" << std::endl;

	std::cout << "Sending response to client" << std::endl;

	visNET::Packet packet;
	packet.writeUInt8(0);
	server.send(receive.begin()->getConnectionIdentifier(), packet);

	// Prevents the console to close
	waitForInput();

	visNET::cleanup();

	return 0;
}
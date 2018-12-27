#include "../../base.h"

int main()
{
	visNET::startup();

	visNET::TcpListener server(8902);
	if (!server.isValid())
	{
		std::cout << "Failed to create listener on port 8902" << std::endl;

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

	server.disconnect(connection);

	std::cout << "Disconnected client" << std::endl;

	// Prevents the console to close
	waitForInput();

	visNET::cleanup();

	return 0;
}
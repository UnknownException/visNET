#include "../../base.h"

int main()
{
	visNET::startup();

	visNET::TcpClient client("127.0.0.1", 8902);
	while (!client.isValid())
	{
		std::cout << "Trying to connect to localhost on port 8902" << std::endl;
		std::this_thread::sleep_for(std::chrono::microseconds(1000));

		client = visNET::TcpClient("127.0.0.1", 8902);
	}

	while (!client.isDisconnected())
	{
		std::this_thread::sleep_for(std::chrono::microseconds(100));
	}

	std::cout << "Disconnected from server" << std::endl;

	// Prevents the console to close
	waitForInput();

	visNET::cleanup();

	return 0;
}
#include "../../base.h"

int main()
{
	visNET::startup();

	visNET::TcpClient client("127.0.0.1", 8901);
	while (!client.isValid())
	{
		std::cout << "Trying to connect to localhost on port 8901" << std::endl;
		std::this_thread::sleep_for(std::chrono::microseconds(1000));

		client = visNET::TcpClient("127.0.0.1", 8901);
	}

	std::cout << "Sending message to server" << std::endl;

	visNET::Packet packet;
	packet.writeUInt8(0);
	client.send(packet);

	auto receive = client.receive();
	while (receive.empty())
	{
		std::this_thread::sleep_for(std::chrono::microseconds(100));

		receive = client.receive();
	}

	std::cout << "Received message from server" << std::endl;

	// Prevents the console to close
	waitForInput();

	visNET::cleanup();

	return 0;
}
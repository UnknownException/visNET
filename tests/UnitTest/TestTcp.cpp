#include "base.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest
{
	TEST_CLASS(TestTcp)
	{
	public:
		/* Basic receiving and sending is tested inside TestPacket*/

		TEST_METHOD(ConstructConnection)
		{
			/* Create TCP listener and client */
			Assert::IsTrue(visNET::startup(), L"Failed to startup", LINE_INFO());

			visNET::TcpListener tcpSvr(TESTTCP_PORT1);
			Assert::IsTrue(tcpSvr.isValid(), L"Failed to initialize TcpListener", LINE_INFO());

			visNET::TcpClient tcpCl("127.0.0.1", TESTTCP_PORT1);
			Assert::IsTrue(tcpCl.isValid(), L"Failed to initialize TcpClient", LINE_INFO());

			/* Try to create second listener on the same port */
			visNET::TcpListener tcpSvrBad(TESTTCP_PORT1);
			Assert::IsFalse(tcpSvrBad.isValid(), L"Managed to create second listener on same port", LINE_INFO());

			/* Try to create a client that connects to a non existing listener */
			visNET::TcpClient tcpClBad("127.0.0.1", TESTTCP_PORT2);
			Assert::IsFalse(tcpClBad.isValid(), L"Managed to connect to an unknown listener", LINE_INFO());

			/* Try to create second listener on a different port */
			visNET::TcpListener tcpSvrSecond(TESTTCP_PORT2);
			Assert::IsTrue(tcpSvrSecond.isValid(), L"Couldn't create second listener", LINE_INFO());

			/* Try to connect to second listener */
			visNET::TcpClient tcpClSecond("127.0.0.1", TESTTCP_PORT2);
			Assert::IsTrue(tcpClSecond.isValid(), L"Failed to connect to second listener", LINE_INFO());
			
			/* Shutdown Winsock */
			Assert::IsTrue(visNET::cleanup(), L"Failed to cleanup", LINE_INFO());
		}
	};
}
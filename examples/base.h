#include "../source/visnet.h"
#include <iostream>
#include <thread>
#include <string>

#ifdef _WIN32
	#ifdef _DEBUG
		#ifdef _DYNAMIC
			#ifdef _WIN64
				#pragma comment(lib, "../../../bin/x64/Debug Dynamic/visNET.lib")
			#else
				#pragma comment(lib, "../../../bin/Win32/Debug Dynamic/visNET.lib")
			#endif
		#else
			#ifdef _WIN64
				#pragma comment(lib, "../../../bin/x64/Debug/visNET.lib")
			#else
				#pragma comment(lib, "../../../bin/Win32/Debug/visNET.lib")
			#endif
		#endif
	#else
		#ifdef _DYNAMIC
			#ifdef _WIN64
				#pragma comment(lib, "../../../bin/x64/Release Dynamic/visNET.lib")
			#else
				#pragma comment(lib, "../../../bin/Win32/Release Dynamic/visNET.lib")
			#endif
		#else
			#ifdef _WIN64
				#pragma comment(lib, "../../../bin/x64/Release/visNET.lib")
			#else
				#pragma comment(lib, "../../../bin/Win32/Release/visNET.lib")
			#endif
		#endif
	#endif
#endif

inline void waitForInput()
{
	std::cout << "Press enter to close the application" << std::endl;
	std::string cin;
	std::cin >> cin;
}
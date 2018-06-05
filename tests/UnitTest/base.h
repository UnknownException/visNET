#pragma once

/*
	This UnitTest project tests all functions exposed by the visNETInterface
*/

#include <CppUnitTest.h>
#include "../../interface/visNETInterface/include.h"
#ifdef _DEBUG
	#pragma comment(lib, "../../Win32/Debug/visNETInterface.lib")
#else
	#pragma comment(lib, "../../Win32/Release/visNETInterface.lib")
#endif

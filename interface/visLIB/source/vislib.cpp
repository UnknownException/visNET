#include "vislib.h"
#include "../../../source/visnet.h"

extern "C"
{
	__declspec(dllexport) bool startup()
	{
		return visNET::startup();
	}

	__declspec(dllexport) bool cleanup()
	{
		return visNET::cleanup();
	}
}
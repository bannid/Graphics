#include <Windows.h>
#include <wingdi.h>
#include "AfricanHead.h"
int CALLBACK WinMain(HINSTANCE instance,
	HINSTANCE prevInstance,
	LPSTR commandLine,
	int showCode)
{
	AfricanHead t;
	if (t.Construct(800,800,1)) {
		t.Start();
	}
	OutputDebugStringA("hey");
	return(0);
}

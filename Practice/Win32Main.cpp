#include <iostream>
#include <Windows.h>
#include <wingdi.h>
#include "TestCode3D.h"

int CALLBACK WinMain(HINSTANCE instance,
	HINSTANCE prevInstance,
	LPSTR commandLine,
	int showCode)
{
	TestCode3D t;
	if (t.Construct(1)) {
		t.Start();
	}
	return(0);
}

#include <iostream>
#include <Windows.h>
#include <wingdi.h>
#include "EarthAndLuna.h"

int CALLBACK WinMain(HINSTANCE instance,
	HINSTANCE prevInstance,
	LPSTR commandLine,
	int showCode)
{
	EarthAndLuna t;
	if (t.Construct(800,800,1)) {
		t.Start();
	}
	return(0);
}

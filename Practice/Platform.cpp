/*This unit will setup the buffer for us to write*/
#include <iostream>
#include <Windows.h>
#include "BEngine.h"
#include "TestCode.h"
#include "DrawLines.h"

int CALLBACK WinMain(HINSTANCE instance,
	HINSTANCE prevInstance,
	LPSTR commandLine,
	int showCode)
{
	Test t;
	if (t.Construct(0,0,1)) {
		t.Start();
	}
	return(0);
}

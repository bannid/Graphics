/*This unit will setup the buffer for us to write*/
#include <iostream>
#include <Windows.h>
#include "BEngine.h"
#include "TestCode.h"
#include "DrawLines.h"
#include "DrawBezierCurves.h"

int CALLBACK WinMain(HINSTANCE instance,
	HINSTANCE prevInstance,
	LPSTR commandLine,
	int showCode)
{
	BezierCurves t;
	if (t.Construct(4)) {
		t.Start();
	}
	return(0);
}

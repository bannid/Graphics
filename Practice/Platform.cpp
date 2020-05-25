#define BENGINE_USE_OPENGL 1

#include <iostream>
#include <Windows.h>
#include <wingdi.h>
#include "BEngine.h"
#include "TestCode.h"
#include "DrawLines.h"
#include "DrawBezierCurves.h"
#include "BouncingBall.h"
#include "SpaceWars.h"
int CALLBACK WinMain(HINSTANCE instance,
	HINSTANCE prevInstance,
	LPSTR commandLine,
	int showCode)
{
	Test t;
	if (t.Construct(600,600,1)) {
		t.Start();
	}
	return(0);
}

#include <iostream>
#include <Windows.h>
#include <wingdi.h>
#include "BEngine.h"
#include "TestCode.h"
#include "DrawLines.h"
#include "DrawBezierCurves.h"
#include "BouncingBall.h"
#include "SpaceWars.h"
#include "RayCaster.h"
#include "DrawFonts.h"
int CALLBACK WinMain(HINSTANCE instance,
	HINSTANCE prevInstance,
	LPSTR commandLine,
	int showCode)
{
	DrawFonts t;
	if (t.Construct(1200,600,1)) {
		t.Start();
	}
	return(0);
}

/*This unit will setup the buffer for us to write*/
#include <iostream>
#include <Windows.h>
#include "BEngine.h"
#include "Renderer.h"
class Test : public BEngine {
	void TestCodeForKeys() {
		NSInput::Key keyA = renderer.GetKey(VK_LBUTTON);
		if (keyA.keyReleased) {
			if (renderer.GetClearColorRGBPacked() == NSRender::YELLOW) {
				renderer.SetClearColor(NSRender::WHITE);
			}
			else {
				renderer.SetClearColor(NSRender::YELLOW);
			}
		}
		renderer.ClearScreen();
	}
	void TestCodeForLerp() {
		NSMath2d::Vec2 point1 = { 0,500 };
		NSMath2d::Vec2 point2 = { 500,500 };
		for (float t = 0; t < 1; t += 0.001) {
			auto temp = renderer.Lerp(point1, point2, t);
			renderer.SetColor(NSRender::YELLOW);
			renderer.SetPixel(temp.x, temp.y);
		}
		renderer.SetColor(NSRender::RED);
		renderer.DrawLine(0, 500, 500, 500);
	}
	void TestCodeForBezierCurveRecursive() {
		renderer.SetClearColor(NSRender::BLACK);
		renderer.ClearScreen();
		renderer.SetColor(NSRender::WHITE);
		float screenHeight = renderer.GetScreenHeight();
		float screenWidth = renderer.GetScreenWidth();
		std::vector<NSMath2d::Vec2> temp = {
				{0,screenHeight / 2},
				{0,0},
				{screenWidth,0},
				{screenWidth,screenHeight / 2}
		};
		NSMath2d::Vec2 currentPoint = temp[0];
		for (float t = 0; t < 1; t += 0.01) {
			NSMath2d::Vec2 temp2(0, 0);
			renderer.BezierCurveRecursive(temp, t, temp2);
			renderer.DrawLine(currentPoint.x, currentPoint.y, temp2.x, temp2.y);
			currentPoint = temp2;
		}
	}
	void TestCodeForMousePos() {
		renderer.SetColor(NSRender::WHITE);
		renderer.SetClearColor(NSRender::BLACK);
		auto mouseCoords = renderer.GetMouseInfo();
		renderer.ClearScreen();
		renderer.DrawLine(0, 0, mouseCoords.x, mouseCoords.y - 31);
	}
	virtual bool OnUpdate() override {
		TestCodeForKeys();
		return true;
	}
};
int CALLBACK WinMain(HINSTANCE instance,
	HINSTANCE prevInstance,
	LPSTR commandLine,
	int showCode)
{
	Test example;
	if (example.Construct()) {
		example.Start();
	}
	return(0);
}

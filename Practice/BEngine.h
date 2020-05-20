#pragma once
#include <Windows.h>
#include "Common.h"
#include "Renderer.h"
class BEngine
{
public:
	BEngine();
	BEngine(LPCWSTR,
		int,
		int);
	~BEngine();
	NSRender::Renderer renderer;
	HWND window;
	WNDCLASS windowClass = {};
	videoMemory_t screenInfo;
	bool running;
	// Get the horizontal and vertical screen sizes in pixel
	void GetDesktopResolution(int&, int&);
	void Win32UpdateWindow(HDC, RECT*, int, int, int, int);
	void Win32ResizeDIBSection(int, int);
	bool Construct();
	bool Start();
private:
	LPCWSTR windowName;
	int windowHeight;
	int windowWidth;
	void TestCodeForMousePos(NSRender::Renderer & renderer) {
		renderer.SetColor(NSRender::WHITE);
		renderer.SetClearColor(NSRender::BLACK);
		auto mouseCoords = renderer.GetMouseInfo();
		renderer.ClearScreen();
		renderer.DrawLine(0, 0, mouseCoords.x, mouseCoords.y - 31);
	}
	void Render(NSRender::Renderer & renderer) {
		TestCodeForMousePos(renderer);
		//TestCodeForKeys(renderer);
		//TestCodeForBezierCurveRecursive(renderer);
		//TestCodeForLerp(renderer);
	}
};


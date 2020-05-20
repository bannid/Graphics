/*This unit will setup the buffer for us to write*/
#include <iostream>
#include <cmath>
#include <vector>
#include <string>
#include <Windows.h>
#include <timeapi.h>
#include <assert.h>
#include "BEngine.h"
#include "Renderer.h"
#include "Common.h"

bool running;
struct videoMemory_t screenInfo;
void Render(NSRender::Renderer & renderer);
void Win32ResizeDIBSection(int Width, int Height)
{
	if (screenInfo.bitmapMemory)
	{
		VirtualFree(screenInfo.bitmapMemory, 0, MEM_RELEASE);
	}
	screenInfo.bitmapWidth = Width;
	screenInfo.bitmapHeight = Height;

	screenInfo.bitmapInfo.bmiHeader.biSize = sizeof(screenInfo.bitmapInfo.bmiHeader);
	screenInfo.bitmapInfo.bmiHeader.biWidth = screenInfo.bitmapWidth;
	screenInfo.bitmapInfo.bmiHeader.biHeight = -screenInfo.bitmapHeight;//Minus to have the y grow downwards
	screenInfo.bitmapInfo.bmiHeader.biPlanes = 1;
	screenInfo.bitmapInfo.bmiHeader.biBitCount = 32;
	screenInfo.bitmapInfo.bmiHeader.biCompression = BI_RGB;
	unsigned int bitmapMemorySize = (screenInfo.bitmapWidth*screenInfo.bitmapHeight)*screenInfo.bytesPerPixel;
	screenInfo.bitmapMemory = VirtualAlloc(0, bitmapMemorySize, MEM_COMMIT, PAGE_READWRITE);
}

void Win32UpdateWindow(HDC deviceContext, RECT* clientRect, int x, int y, int width, int height)
{
	int windowWidth = clientRect->right - clientRect->left;
	int windowHeight = clientRect->bottom - clientRect->top;
	StretchDIBits(deviceContext,
		0, 0, screenInfo.bitmapWidth, screenInfo.bitmapHeight,
		0, 0, windowWidth, windowHeight,
		screenInfo.bitmapMemory,
		&screenInfo.bitmapInfo,
		DIB_RGB_COLORS, SRCCOPY);
}
LRESULT CALLBACK Win32MainWindowCallback(HWND window,
	UINT message,
	WPARAM wParam,
	LPARAM lParam)
{
	LRESULT result = 0;

	switch (message)
	{
	case WM_SIZE:
	{
		RECT clientRect;
		GetClientRect(window, &clientRect);
		int width = clientRect.right - clientRect.left;
		int height = clientRect.bottom - clientRect.top;
		Win32ResizeDIBSection(width, height);
	} break;
	case WM_KEYDOWN: {
	}break;
	case WM_CLOSE:
	{
		running = false;
	} break;

	case WM_ACTIVATEAPP:
	{
		OutputDebugStringA("WM_ACTIVATEAPP\n");
	} break;

	case WM_DESTROY:
	{
		running = false;
	} break;

	case WM_PAINT:
	{
		PAINTSTRUCT paint;
		HDC deviceContext = BeginPaint(window, &paint);
		int x = paint.rcPaint.left;
		int y = paint.rcPaint.top;
		int width = paint.rcPaint.right - paint.rcPaint.left;
		int Height = paint.rcPaint.bottom - paint.rcPaint.top;

		RECT clientRect;
		GetClientRect(window, &clientRect);

		Win32UpdateWindow(deviceContext, &clientRect, x, y, width, Height);
		EndPaint(window, &paint);
	} break;

	default:
	{
		result = DefWindowProc(window, message, wParam, lParam);
	} break;
	}

	return(result);
}
// Get the horizontal and vertical screen sizes in pixel
void GetDesktopResolution(int& horizontal, int& vertical)
{
	RECT desktop;
	// Get a handle to the desktop window
	const HWND hDesktop = GetDesktopWindow();
	// Get the size of screen to the variable desktop
	GetWindowRect(hDesktop, &desktop);
	// The top left corner will have coordinates (0,0)
	// and the bottom right corner will have coordinates
	// (horizontal, vertical)
	horizontal = desktop.right;
	vertical = desktop.bottom;
}
void CreateWindowCustom(HWND & window) {
	WNDCLASS windowClass = {};
	windowClass.lpfnWndProc = Win32MainWindowCallback;
	windowClass.hInstance = GetModuleHandle(nullptr);
	windowClass.lpszClassName = L"ConsoleClass";
	int windowWidth = 0;
	int windowHeight = 0;
	GetDesktopResolution(windowWidth, windowHeight);
	if (RegisterClass(&windowClass))
	{
		window =
			CreateWindowEx(
				0,
				windowClass.lpszClassName,
				L"Banni Engine",
				WS_OVERLAPPEDWINDOW | WS_VISIBLE,
				0,
				0,
				windowWidth,
				windowHeight,
				0,
				0,
				GetModuleHandle(nullptr),
				0);
	}
}
int CALLBACK WinMain(HINSTANCE instance,
	HINSTANCE prevInstance,
	LPSTR commandLine,
	int showCode)
{
	BEngine b;
	b.Construct();
	b.Start();
	return(0);
}
void TestCodeForBezierCurveRecursive(NSRender::Renderer&);
void TestCodeForLerp(NSRender::Renderer &);
void TestCodeForKeys(NSRender::Renderer &);
void TestCodeForMousePos(NSRender::Renderer&);
void Render(NSRender::Renderer & renderer) {
	TestCodeForMousePos(renderer);
	//TestCodeForKeys(renderer);
	//TestCodeForBezierCurveRecursive(renderer);
	//TestCodeForLerp(renderer);
}
void TestCodeForMousePos(NSRender::Renderer & renderer) {
	renderer.SetColor(NSRender::WHITE);
	renderer.SetClearColor(NSRender::BLACK);
	auto mouseCoords = renderer.GetMouseInfo();
	renderer.ClearScreen();
	renderer.DrawLine(0, 0, mouseCoords.x, mouseCoords.y - 31);
}
void TestCodeForKeys(NSRender::Renderer & renderer) {
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
void TestCodeForLerp(NSRender::Renderer & renderer) {
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
void TestCodeForBezierCurveRecursive(NSRender::Renderer & renderer) {
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

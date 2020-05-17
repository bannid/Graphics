/*This unit will setup the buffer for us to write*/
#include <iostream>
#include <cmath>
#include <string>
#include <Windows.h>
#include <timeapi.h>
#include <assert.h>
#include "Renderer.h"
#include "Common.h"

bool running;
struct videoMemory_t screenInfo;
Renderer renderer;
void Render();
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
int CALLBACK WinMain(HINSTANCE instance,
	HINSTANCE prevInstance,
	LPSTR commandLine,
	int showCode)
{
	WNDCLASS windowClass = {};
	windowClass.lpfnWndProc = Win32MainWindowCallback;
	windowClass.hInstance = instance;
	windowClass.lpszClassName = L"ConsoleClass";
	int windowWidth = 0;
	int windowHeight = 0;
	GetDesktopResolution(windowWidth, windowHeight);
	if (RegisterClass(&windowClass))
	{
		HWND window =
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
				instance,
				0);
		if (window)
		{
			running = true;
			renderer.SetVideoMemory(&screenInfo);
			while (running)
			{
				MSG message;
				while (PeekMessage(&message, 0, 0, 0, PM_REMOVE))
				{
					if (message.message == WM_QUIT)
					{
						running = false;
					}
					TranslateMessage(&message);
					DispatchMessageA(&message);
				}
				RECT clientRect;
				GetClientRect(window, &clientRect);
				int windowWidth = clientRect.right - clientRect.left;
				int windowHeight = clientRect.bottom - clientRect.top;
				Render();
				HDC DeviceContext = GetDC(window);
				Win32UpdateWindow(DeviceContext, &clientRect, 0, 0, windowWidth, windowHeight);
				ReleaseDC(window, DeviceContext);
			}
		}
		else
		{

		}
	}
	else
	{

	}
	return(0);
}

void Render() {
	int startingX = 200;
	int endingX = 10;
	int startingY = 500;
	int endingY = 10;
	renderer.ClearScreen();
	renderer.SetColor({ 0,0,0 });
	renderer.DrawLine(startingX, startingY, endingX, endingY);
	renderer.SetColor({ 255,0,0 });
	renderer.SetPixel(startingX, startingY);
	renderer.SetPixel(endingX, endingY);
}
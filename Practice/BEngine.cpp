#include "BEngine.h"
static BEngine * static_enginePtr = nullptr;
static LRESULT CALLBACK static_Win32MainWindowCallback(HWND window,
	UINT message,
	WPARAM wParam,
	LPARAM lParam) {
	LRESULT result = 0;

	switch (message)
	{
	case WM_SIZE:
	{
		RECT clientRect;
		GetClientRect(window, &clientRect);
		int width = clientRect.right - clientRect.left;
		int height = clientRect.bottom - clientRect.top;
		static_enginePtr->Win32ResizeDIBSection(width, height);
	} break;
	case WM_KEYDOWN: {
	}break;
	case WM_CLOSE:
	{
		static_enginePtr->running = false;
	} break;

	case WM_ACTIVATEAPP:
	{
		OutputDebugStringA("WM_ACTIVATEAPP\n");
	} break;

	case WM_DESTROY:
	{
		static_enginePtr = false;
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

		static_enginePtr->Win32UpdateWindow(deviceContext,
			&clientRect,
			x,
			y,
			width,
			Height);
		EndPaint(window, &paint);
	} break;

	default:
	{
		result = DefWindowProc(window, message, wParam, lParam);
	} break;
	}

	return(result);
}
void BEngine::GetDesktopResolution(int& horizontal, int& vertical)
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
void BEngine::Win32UpdateWindow(HDC deviceContext, RECT* clientRect, int x, int y, int width, int height)
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
void BEngine::Win32ResizeDIBSection(int Width, int Height)
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
BEngine::BEngine(LPCWSTR windowName, int width, int height):windowName(windowName),
windowHeight(height),
windowWidth(width){
	
}
bool BEngine::Start() {
	if (!window) {
		return false;
	}
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
		renderer.ProcessKeys();
		Render(renderer);
		HDC DeviceContext = GetDC(window);
		Win32UpdateWindow(DeviceContext, &clientRect, 0, 0, windowWidth, windowHeight);
		ReleaseDC(window, DeviceContext);
	}
	return true;
}

bool BEngine::Construct() {
	running = true;
	static_enginePtr = this;
	renderer = NSRender::Renderer(&this->screenInfo, 1);
	windowClass.lpfnWndProc = static_Win32MainWindowCallback;
	windowClass.hInstance = GetModuleHandle(nullptr);
	windowClass.lpszClassName = L"ConsoleClass";
	GetDesktopResolution(windowWidth, windowHeight);
	if (RegisterClass(&windowClass))
	{
		window =
			CreateWindowEx(
				0,
				windowClass.lpszClassName,
				windowName,
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
	return true;
}
BEngine::BEngine()
{
	
}


BEngine::~BEngine()
{
}

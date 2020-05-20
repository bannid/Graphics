#include <assert.h>
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
		ProcessKeys();
		OnUpdate();
		//Render(renderer);
		HDC DeviceContext = GetDC(window);
		Win32UpdateWindow(DeviceContext, &clientRect, 0, 0, windowWidth, windowHeight);
		ReleaseDC(window, DeviceContext);
	}
	return true;
}
bool BEngine::Construct() {
	//This will cover the whole screen with our window.
	return this->Construct(0, 0, 1);
}
bool BEngine::Construct(int windowWidth,int windowHeight, int pixelDimensions) {
	pixelDimension = pixelDimensions;
	running = true;
	static_enginePtr = this;
	windowClass.lpfnWndProc = static_Win32MainWindowCallback;
	windowClass.hInstance = GetModuleHandle(nullptr);
	windowClass.lpszClassName = L"ConsoleClass";
	if (windowHeight == 0 || windowWidth == 0) {
		GetDesktopResolution(this->windowWidth, this->windowHeight);
	}
	else {
		this->windowHeight = windowHeight;
		this->windowWidth = windowWidth;
	}
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
				this->windowWidth,
				this->windowHeight,
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


	void BEngine::SetColor(color_t color) {
		this->color = color;
	}
	int BEngine::GetScreenHeight() {
		return this->screenInfo.bitmapHeight;
	}
	int BEngine::GetScreenWidth() {
		return this->screenInfo.bitmapWidth;
	}
	color_t BEngine::GetClearColor() {
		return this->clearColor;
	}
	color_t BEngine::GetColor() {
		return this->color;
	}
	POINT BEngine::GetMouseInfo() {
		return this->mouseInfo;
	}
	unsigned int BEngine::GetColorRGBPacked() {
		return this->RGBPackedColor;
	}
	unsigned int BEngine::GetClearColorRGBPacked() {
		return this->RGBPackedClearColor;
	}
	void BEngine::SetColor(int rgb) {
		this->RGBPackedColor = rgb;
		int red = rgb >> 16 & 0xFF;
		int green = rgb >> 8 & 0xFF;
		int blue = rgb & 0xFF;
		this->color.red = red;
		this->color.blue = blue;
		this->color.green = green;
	}
	void BEngine::SetClearColor(color_t color) {
		this->clearColor = color;
	}
	float BEngine::Lerp(float a, float b, float t) {
		return ((1 - t) * a) + (t * b);
	}
	void BEngine::SetClearColor(int rgb) {
		this->RGBPackedClearColor = rgb;
		int red = rgb >> 16 & 0xFF;
		int green = rgb >> 8 & 0xFF;
		int blue = rgb & 0xFF;
		this->clearColor.red = red;
		this->clearColor.blue = blue;
		this->clearColor.green = green;
	}
	void BEngine::SetPixelInternal(int x, int y) {
		if (x < 0 || x >= this->screenInfo.bitmapWidth || y < 0 || y >= this->screenInfo.bitmapHeight) {
			return;
		}
		int memorySize = this->screenInfo.bitmapHeight * this->screenInfo.bitmapWidth;
		unsigned int * pixel = (unsigned int *)this->screenInfo.bitmapMemory;
		unsigned int toPlus = x + this->screenInfo.bitmapWidth * y;
		pixel += toPlus;
		*pixel = (((this->color.red << 8) | this->color.green) << 8) | this->color.blue;
	}
	void BEngine::SetPixel(int x, int y) {
		for (int i = x; i < x + this->pixelDimension; i++) {
			for (int k = y; k < y + this->pixelDimension; k++) {
				SetPixelInternal(i, k);
			}
		}
	}
	void BEngine::ClearScreen() {
		unsigned int memorySize = this->screenInfo.bitmapWidth * this->screenInfo.bitmapHeight;
		unsigned int * pixel = (unsigned int *)this->screenInfo.bitmapMemory;
		for (unsigned int i = 0; i < memorySize; i++) {
			*pixel = (((this->clearColor.red << 8) | this->clearColor.green) << 8) | this->clearColor.blue;
			pixel++;
		}
	}

	void BEngine::DrawLine(int x1, int y1, int x2, int y2) {
		bool steep = false;
		if (std::abs(y2 - y1) > std::abs(x2 - x1)) {
			steep = true;
			std::swap(x1, y1);
			std::swap(x2, y2);
		}
		if (x1 > x2) {
			std::swap(x2, x1);
			std::swap(y2, y1);
		}
		float dy = std::abs(y2 - y1);
		float dx = std::abs(x2 - x1);
		float slope = dy / dx;
		float delta = 0.0;
		unsigned int y = y1;
		for (int x = x1; x < x2; x++) {
			if (!steep) {
				SetPixel(x, y);
			}
			else {
				SetPixel(y, x);
			}
			delta += slope;
			if (delta > 0.5) {
				delta -= 1;
				y += (y2 - y1) > 0 ? 1 : -1;
			}
		}
	}
	NSMath2d::Vec2 BEngine::QuadraticBezierCurve(NSMath2d::Vec2 p1,
		NSMath2d::Vec2 cp,
		NSMath2d::Vec2 p2,
		float t) {
		auto s = GetTwoLinearPointsFromThreePoints(p1, cp, p2, t);
		NSMath2d::Vec2 finalVector = s[1] - s[0];
		NSMath2d::Vec2 finalVectorScaled = finalVector * t;
		return s[0] + finalVectorScaled;
	}
	//Helper function to draw Bezier Curve.  Given three points, we get two points back
	//in respect the value of t
	std::vector<NSMath2d::Vec2> BEngine::GetTwoLinearPointsFromThreePoints(NSMath2d::Vec2 p1, NSMath2d::Vec2 p2, NSMath2d::Vec2 p3, float t) {
		std::vector<NSMath2d::Vec2> toReturn;
		//Get first vector
		NSMath2d::Vec2 vectorFromP1ToP2 = p2 - p1;
		//Get second vector
		NSMath2d::Vec2 vectorFromP2ToP3 = p3 - p2;
		//Scale both of the vectors in respect to t
		NSMath2d::Vec2 vectorFromP1ToP2Scaled = vectorFromP1ToP2 * t;
		NSMath2d::Vec2 vectorFromP2ToP3Scaled = vectorFromP2ToP3 * t;
		//Add the scaled vector to the starting points to get a new point.
		//Remember, we need to add them because we have directions not positions.
		//We get positions by adding the directional vectors on the original positions
		NSMath2d::Vec2 point1 = p1 + vectorFromP1ToP2Scaled;
		NSMath2d::Vec2 point2 = p2 + vectorFromP2ToP3Scaled;
		//Return the two points
		toReturn.push_back(point1);
		toReturn.push_back(point2);
		return toReturn;
	}
	void BEngine::BezierCurveRecursive(std::vector<NSMath2d::Vec2> points,
		float t,
		NSMath2d::Vec2 & output) {
		if (points.size() == 3) {
			NSMath2d::Vec2 toReturn = QuadraticBezierCurve(points[0], points[1], points[2], t);
			output.x = toReturn.x;
			output.y = toReturn.y;
		}
		else if (points.size() > 3) {
			std::vector<NSMath2d::Vec2> run;
			for (int i = 0; i < points.size() - 2; i++) {
				auto temp = GetTwoLinearPointsFromThreePoints(points[i], points[i + 1], points[i + 2], t);
				if (i == 0) {
					for (int i = 0; i < temp.size(); i++) {
						run.push_back(temp[i]);
					}
				}
				else {
					for (int i = 1; i < temp.size(); i++) {
						run.push_back(temp[i]);
					}
				}
			}
			BezierCurveRecursive(run, t, output);
		}
	}
	NSMath2d::Vec2 BEngine::Lerp(NSMath2d::Vec2 a, NSMath2d::Vec2 b, float t) {
		NSMath2d::Vec2 vectorFromAToB = b - a;
		NSMath2d::Vec2 vectorFromAToBScaled = vectorFromAToB * t;
		return a + vectorFromAToBScaled;
	}
	void BEngine::ProcessKeys() {
		for (int key = 0; key < 0xff; key++) {
			if (!this->keys[key].keyDown) {
				this->keys[key].keyDown = 0x1 & (GetAsyncKeyState(key) >> 15);
				this->keys[key].keyReleased = false;
			}
			else {
				this->keys[key].keyDown = (0x1 & (GetAsyncKeyState(key) >> 15));
				if (this->keys[key].keyDown) {
					this->keys[key].keyHeld = true;
					this->keys[key].keyReleased = false;
				}
				else {
					this->keys[key].keyHeld = false;
					this->keys[key].keyReleased = true;
				}
			}
		}
		GetCursorPos(&this->mouseInfo);
	}
	NSInput::Key BEngine::GetKey(unsigned int key) {
		assert(key > 0 && key < 0xFF);
		return this->keys[key];
	}

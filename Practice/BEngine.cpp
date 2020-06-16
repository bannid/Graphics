#include "BEngine.h"
#include "Utils.h"
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
		static_enginePtr->WriteTimingOutput();
		static_enginePtr->running = false;
	} break;

	case WM_ACTIVATEAPP:
	{
		OutputDebugStringA("WM_ACTIVATEAPP\n");
	} break;

	case WM_DESTROY:
	{
		static_enginePtr->running = false;
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
bool BEngine::InitOpenGl()
{
	HDC WindowDC = GetDC(window);

	PIXELFORMATDESCRIPTOR DesiredPixelFormat = {};
	DesiredPixelFormat.nSize = sizeof(DesiredPixelFormat);
	DesiredPixelFormat.nVersion = 1;
	DesiredPixelFormat.iPixelType = PFD_TYPE_RGBA;
	DesiredPixelFormat.dwFlags = PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW | PFD_DOUBLEBUFFER;
	DesiredPixelFormat.cColorBits = 32;
	DesiredPixelFormat.cAlphaBits = 8;
	DesiredPixelFormat.iLayerType = PFD_MAIN_PLANE;

	int SuggestedPixelFormatIndex = ChoosePixelFormat(WindowDC, &DesiredPixelFormat);
	PIXELFORMATDESCRIPTOR SuggestedPixelFormat;
	DescribePixelFormat(WindowDC, SuggestedPixelFormatIndex,
		sizeof(SuggestedPixelFormat), &SuggestedPixelFormat);
	SetPixelFormat(WindowDC, SuggestedPixelFormatIndex, &SuggestedPixelFormat);

	HGLRC OpenGLRC = wglCreateContext(WindowDC);
	if (wglMakeCurrent(WindowDC, OpenGLRC))
	{
		glGenTextures(1, &blitTextureHandle);
	}
	else
	{
		return false;
	}
	ReleaseDC(window, WindowDC);
	return true;
}

void BEngine::Win32UpdateWindowOpenGL(HDC deviceContext, RECT* clientRect, int x, int y, int width, int height) {
	int windowWidth = clientRect->right - clientRect->left;
	int windowHeight = clientRect->bottom - clientRect->top;
	glViewport(0, 0, windowWidth, windowHeight);

	glBindTexture(GL_TEXTURE_2D, blitTextureHandle);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, screenInfo.bitmapWidth, screenInfo.bitmapHeight, 0,
		GL_BGRA_EXT, GL_UNSIGNED_BYTE, screenInfo.bitmapMemory);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	glEnable(GL_TEXTURE_2D);

	glClearColor(1.0f, 0.0f, 1.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glMatrixMode(GL_TEXTURE);
	glLoadIdentity();

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glBegin(GL_TRIANGLES);

	float P = 1.0f;

	glTexCoord2f(0.0f, 1.0f);
	glVertex2f(-P, -P);

	glTexCoord2f(1.0f, 1.0f);
	glVertex2f(P, -P);

	glTexCoord2f(1.0f, 0.0f);
	glVertex2f(P, P);

	glTexCoord2f(0.0f, 1.0f);
	glVertex2f(-P, -P);

	glTexCoord2f(1.0f, 0.0f);
	glVertex2f(P, P);

	glTexCoord2f(0.0f, 0.0f);
	glVertex2f(-P, P);

	glEnd();

	SwapBuffers(deviceContext);
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
		VirtualFree(screenInfo.zBuffer, 0, MEM_RELEASE);
	}
	screenInfo.bitmapWidth = Width;
	screenInfo.bitmapHeight = Height;

	screenInfo.bitmapInfo.bmiHeader.biSize = sizeof(screenInfo.bitmapInfo.bmiHeader);
	screenInfo.bitmapInfo.bmiHeader.biWidth = screenInfo.bitmapWidth;
	screenInfo.bitmapInfo.bmiHeader.biHeight = screenInfo.bitmapHeight;//Minus to have the y grow downwards
	screenInfo.bitmapInfo.bmiHeader.biPlanes = 1;
	screenInfo.bitmapInfo.bmiHeader.biBitCount = 32;
	screenInfo.bitmapInfo.bmiHeader.biCompression = BI_RGB;
	unsigned int bitmapMemorySize = (screenInfo.bitmapWidth*screenInfo.bitmapHeight)*screenInfo.bytesPerPixel;
	screenInfo.bitmapMemory = VirtualAlloc(0, bitmapMemorySize, MEM_COMMIT, PAGE_READWRITE);
	screenInfo.zBuffer = VirtualAlloc(0, bitmapMemorySize, MEM_COMMIT, PAGE_READWRITE);
	ClearZBuffer();
}
bool BEngine::OnDestroy() {
	return true;
}
bool BEngine::Start() {		
	if (!InitOpenGl()) { return false; }
	this->uct1 = std::chrono::steady_clock::now();
	this->uct2 = std::chrono::steady_clock::now();
	while (running)
	{
		TIMED_DATA;
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
		this->uct2 = std::chrono::steady_clock::now();
		float elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(uct2 - uct1).count();
		MILISECONDS_TO_SEC(elapsedTime);
		running = running && OnUpdate(elapsedTime);
		this->uct1 = this->uct2;
		std::string s = std::to_string(this->mouseInfo.x);
		s.append(",");
		s.append(std::to_string(this->mouseInfo.y));
		SetWindowTextA(window,s.c_str());
		HDC DC = GetDC(window);
		Win32UpdateWindowOpenGL(DC, &clientRect, 0, 0, windowWidth, windowHeight);
		ReleaseDC(window, DC);
		if (!running) { running = !OnDestroy(); }
	}
	return true;
}
bool BEngine::Construct(int pixelDimensions) {
	//This will cover the whole screen with our window.
	GetDesktopResolution(this->windowWidth, this->windowHeight);
	return this->Construct(this->windowWidth, this->windowHeight, pixelDimensions);
}
bool BEngine::Construct(int windowWidth, int windowHeight, int pixelDimensions) {
	this->windowWidth = windowWidth;
	this->windowHeight = windowHeight;
	pixelDimension = pixelDimensions;

	if (!BUtils::LoadTexturePNG("C:\\Users\\Winny-Banni\\Pictures\\Fonts.png", &this->fontsTexture)) {
		//Implement a way to know why it failed
		return false;
	}
	running = true;
	static_enginePtr = this;
	windowClass.lpfnWndProc = static_Win32MainWindowCallback;
	windowClass.hInstance = GetModuleHandle(nullptr);
	windowClass.lpszClassName = L"ConsoleClass";
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
		if (!window) {
			return false;
		}
	}
	else {
		return false;
	}
	return OnCreate();
}
BEngine::BEngine()
{

}
BEngine::~BEngine()
{

}
//Getters
BInput::Key BEngine::GetKey(unsigned int key) {
	assert(key > 0 && key < 0xFF);
	return this->keys[key];
}
float BEngine::GetZBuffer(int x, int y) {
	float * ptr = (float*)this->screenInfo.zBuffer;
	return ptr[x + y * this->screenInfo.bitmapWidth];
}
int BEngine::GetPixelDimension() {
	return this->pixelDimension;
}
int BEngine::GetScreenHeight() {
	return this->screenInfo.bitmapHeight;
}
int BEngine::GetScreenWidth() {
	return this->screenInfo.bitmapWidth;
}
POINT BEngine::GetMouseInfo() {
	return this->mouseInfo;
}
//Setters
void BEngine::SetBlendingMode(BLENDING_MODE mode) { this->blendMode = mode; }
void BEngine::SetPixelInternal(int x, int y, BColors::color_t & color) {
	if (x < 0 || x >= this->screenInfo.bitmapWidth || y < 0 || y >= this->screenInfo.bitmapHeight) {
		return;
	}
	switch (blendMode) {
	case NORMAL:
	{
		unsigned int * pixel = (unsigned int *)this->screenInfo.bitmapMemory;
		pixel[x + this->screenInfo.bitmapWidth * y] = (((color.red << 8) | color.green) << 8) | color.blue;
		break;
	}
	case ALPHA: {
		unsigned int * pixel = (unsigned int *)this->screenInfo.bitmapMemory;
		unsigned int pixelValue = pixel[x + this->screenInfo.bitmapWidth * y];
		float alpha = color.alpha / 255.0f;
		float alpha2 = 1 - alpha;
		int finalRed = alpha * color.red + alpha2 * (pixelValue >> 16 & 0xFF);
		int finalGreen = alpha * color.green + alpha2 * (pixelValue >> 8 & 0xFF);
		int finalBlue = alpha * color.blue + alpha2 * (pixelValue & 0xFF);
		pixel[x + this->screenInfo.bitmapWidth * y] = (((finalRed << 8) | finalGreen) << 8) | finalBlue;
		break;
	}
	}
}
void BEngine::SetPixel(int x, int y, BColors::color_t color) {
	for (int i = x; i < x + this->pixelDimension; i++) {
		for (int k = y; k < y + this->pixelDimension; k++) {
			SetPixelInternal(i, k, color);
		}
	}
}
void BEngine::SetPixel(int x, int y, int colorPacked) {
	SetPixel(x, y, IntToColor(colorPacked));
}
void BEngine::SetZBuffer(int x, int y, float value) {
	float * ptr = (float*)this->screenInfo.zBuffer;
	ptr[x + y * this->screenInfo.bitmapWidth] = value;
}
void BEngine::ClearScreen(BColors::color_t & color) {
	unsigned int memorySize = this->screenInfo.bitmapWidth * this->screenInfo.bitmapHeight;
	unsigned int * pixel = (unsigned int *)this->screenInfo.bitmapMemory;
	for (unsigned int i = 0; i < memorySize; i++) {
		*pixel = (((color.red << 8) | color.green) << 8) | color.blue;
		pixel++;
	}
}
void BEngine::ClearScreenWithTexture(Texture * texture) {
	int screenHeight = GetScreenHeight();
	int screenWidth = GetScreenWidth();
	for (int x = 0; x < screenWidth; x++) {
		for (int y = 0; y < screenHeight; y++) {
			float normalizedX = (float)x / screenWidth;
			float normalizedY = (float)y / screenHeight;
			BColors::color_t color = BUtils::GetColorFromTexture(normalizedX, normalizedY, texture);
			SetPixel(x, y, color);
		}
	}
}
void BEngine::ClearZBuffer() {
	for (int x = 0; x < screenInfo.bitmapWidth; x++) {
		for (int y = 0; y < screenInfo.bitmapHeight; y++) {
			float * ptr = (float*)screenInfo.zBuffer;
			ptr[x + y * screenInfo.bitmapWidth] = 100000.0f;
		}
	}
}
void BEngine::ClearScreen(int colorPacked) {
	BColors::color_t color = IntToColor(colorPacked);
	ClearScreen(color);
}
//############################## Circles ###############################
void BEngine::DrawCircle(int x, int y, int radius, BColors::color_t & color) {
	float tau = 6.28318530718;
	for (float t = 0; t < 1; t += 0.001) {
		int x1 = std::cos(t * tau) * radius;
		int y1 = std::sin(t * tau) * radius;
		SetPixel(x + x1, y + y1, color);
	}
}
void BEngine::DrawCircle(int x, int y, int radius, int colorPacked) {
	BColors::color_t color = IntToColor(colorPacked);
	DrawCircle(x, y, radius, color);
}
void BEngine::DrawCircle(BMath::Vec2 & point, int radius, BColors::color_t & color) {
	DrawCircle(point.x, point.y, radius, color);
}
void BEngine::DrawCircle(BMath::Vec2 & pos, int radius, int colorPacked) {
	BColors::color_t color = IntToColor(colorPacked);
	DrawCircle(pos.x, pos.y, radius, color);
}
void BEngine::FillCircle(const BMath::Vec2 & point, int radius, BColors::color_t & color) {
	this->FillCircle(point.x, point.y, radius, color);
}
void BEngine::FillCircle(const BMath::Vec2 & pos, int radius, int colorPacked) {
	BColors::color_t color = IntToColor(colorPacked);
	this->FillCircle(pos.x, pos.y, radius, color);
}
void BEngine::FillCircle(int x, int y, int radius, int colorPacked) {
	BColors::color_t color = IntToColor(colorPacked);
	this->FillCircle(x, y, radius, color);
}
void BEngine::FillCircle(int x, int y, int radius, BColors::color_t & color) {
	int x1 = x - radius;
	int y1 = y - radius;
	int x2 = radius + x;
	int y2 = radius + y;
	for (int xi = x1; xi < x2; xi++) {
		for (int yi = y1; yi < y2; yi++) {
			int dx = abs(x - xi);
			int dy = abs(y - yi);
			if (sqrt(dx * dx + dy * dy) < radius) {
				SetPixel(xi, yi, color);
			}
		}
	}
}
//############################### End Circles ############################
//############################## Rectangle ############################
void BEngine::DrawRectangle(int x1, int y1, int x2, int y2, BColors::color_t & color) {
	DrawLine(x1, y1, x2, y1, color);
	DrawLine(x1, y1, x1, y2, color);
	DrawLine(x1, y2, x2, y2, color);
	DrawLine(x2, y1, x2, y2, color);
}
void BEngine::DrawRectangle(int xTop, int yTop, int xBottom, int yBottom, int colorPacked) {
	BColors::color_t color = IntToColor(colorPacked);
	DrawLine(xTop, yTop, xBottom, yTop, color);
	DrawLine(xTop, yTop, xTop, yBottom, color);
	DrawLine(xTop, yBottom, xBottom, yBottom, color);
	DrawLine(xBottom, yTop, xBottom, yBottom, color);
}
void BEngine::FillRectangle(int x1, int y1, int x2, int y2, BColors::color_t & color) {
	for (int y = y1; y < y2; y++) {
		DrawLine(x1, y, x2, y, color);
	}
}
void BEngine::FillRectangle(int xTop, int yTop, int xBottom, int yBottom, int colorPacked) {
	BColors::color_t color = IntToColor(colorPacked);
	FillRectangle(xTop, yTop, xBottom, yBottom, color);
}
//############################## End rectangle #################################
//################################ Triangle ##############################
void BEngine::DrawTriangle(Triangle & t) {
	auto & v1 = t.vertices[0].vector;
	auto & v2 = t.vertices[1].vector;
	auto & v3 = t.vertices[2].vector;

	DrawLine(v1.x, v1.y, v2.x, v2.y, BColors::WHITE);
	DrawLine(v2.x, v2.y, v3.x, v3.y, BColors::WHITE);
	DrawLine(v3.x, v3.y, v1.x, v1.y, BColors::WHITE);
}
void BEngine::DrawTriangle(Vertex & one, Vertex & two, Vertex & three) {
	auto & v1 = one.vector;
	auto & v2 = two.vector;
	auto & v3 = three.vector;

	DrawLine(v1.x, v1.y, v2.x, v2.y, BColors::WHITE);
	DrawLine(v2.x, v2.y, v3.x, v3.y, BColors::WHITE);
	DrawLine(v3.x, v3.y, v1.x, v1.y, BColors::WHITE);
}
//Fill triangle - Scanline method
void BEngine::FillTriangle(Triangle & t) {
	auto & v1 = t.vertices[0].vector;
	auto & v2 = t.vertices[1].vector;
	auto & v3 = t.vertices[2].vector;
	int x1 = v1.x, y1 = v1.y, x2 = v2.x, y2 = v2.y, x3 = v3.x, y3 = v3.y;
	if (y1 > y2) {
		std::swap(y1, y2);
		std::swap(x1, x2);
	}
	if (y2 > y3) {
		std::swap(y2, y3);
		std::swap(x2, x3);
	}
	if (y1 > y2) {
		std::swap(y1, y2);
		std::swap(x1, x2);
	}
	int triangleHeight = y3 - y1;
	if (triangleHeight == 0) { return; }//Dont draw triangles that are not triangles.
	int segmentHeight = y2 - y1;
	float percision = 0.4f;
	//Draw the upper part
	for (int y = y1; y < y2; y++) {
		float alpha = (float)(y3 - y) / triangleHeight;
		float beta = (float)(y2 - y) / segmentHeight;
		int xFirst = beta * x1 + (1.0f - beta) * x2 + percision;
		int xSecond = alpha * x1 + (1.0f - alpha) * x3 + percision;
		DrawLine(xFirst, y, xSecond, y, t.vertices[0].color);
	}
	//Draw the lower part
	segmentHeight = y3 - y2;
	for (int y = y2; y < y3; y++) {
		float alpha = (float)(y3 - y) / triangleHeight;
		float beta = (float)(y3 - y) / segmentHeight;
		int xSecond = alpha * x1 + (1 - alpha) * x3 + percision;
		int xFirst = beta * x2 + (1 - beta) * x3 + percision;
		DrawLine(xFirst, y, xSecond, y, t.vertices[0].color);
	}
}
//######################## End triangle##################################
//######################### Lines and Curves ############################
void BEngine::DrawLine(int x1, int y1, int x2, int y2, int colorPacked) {
	BColors::color_t color = IntToColor(colorPacked);
	DrawLine(x1, y1, x2, y2, color);
}
void BEngine::DrawLine(int x1, int y1, int x2, int y2, BColors::color_t & color) {
	bool steep = false;
	//TODO: Remove once clipping is done.
	x1 = BUtils::Max(0, x1);
	x1 = BUtils::Min(GetScreenWidth(), x1);
	y1 = BUtils::Max(0, y1);
	y1 = BUtils::Min(GetScreenHeight(), y1);

	x2 = BUtils::Max(0, x2);
	x2 = BUtils::Min(GetScreenWidth(), x2);
	y2 = BUtils::Max(0, y2);
	y2 = BUtils::Min(GetScreenHeight(), y2);
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
			SetPixel(x, y, color);
		}
		else {
			SetPixel(y, x, color);
		}
		delta += slope;
		if (delta > 0.5) {
			delta -= 1;
			y += (y2 - y1) > 0 ? 1 : -1;
		}
	}
}
void BEngine::DrawBezierCurve(BMath::Vec2 p1, BMath::Vec2 cp, BMath::Vec2 p2, BColors::color_t & color) {
	auto currentPoint = p1;
	for (float t = 0; t <= 1.05; t += 0.05f) {
		BMath::Vec2 temp2(0, 0);
		temp2 = QuadraticBezierCurve(p1, cp, p2, t);
		DrawLine(currentPoint.x, currentPoint.y, temp2.x, temp2.y, color);
		currentPoint = temp2;
	}
}
BMath::Vec2 BEngine::QuadraticBezierCurve(BMath::Vec2 p1,
	BMath::Vec2 cp,
	BMath::Vec2 p2,
	float t) {
	auto s = GetTwoLinearPointsFromThreePoints(p1, cp, p2, t);
	BMath::Vec2 finalVector = s[1] - s[0];
	BMath::Vec2 finalVectorScaled = finalVector * t;
	return s[0] + finalVectorScaled;
}
//Helper function to draw Bezier Curve.  Given three points, we get two points back
//in respect the value of t
std::vector<BMath::Vec2> BEngine::GetTwoLinearPointsFromThreePoints(BMath::Vec2 p1,
	BMath::Vec2 p2,
	BMath::Vec2 p3, float t) {
	std::vector<BMath::Vec2> toReturn;
	//Get first vector
	BMath::Vec2 vectorFromP1ToP2 = p2 - p1;
	//Get second vector
	BMath::Vec2 vectorFromP2ToP3 = p3 - p2;
	//Scale both of the vectors in respect to t
	BMath::Vec2 vectorFromP1ToP2Scaled = vectorFromP1ToP2 * t;
	BMath::Vec2 vectorFromP2ToP3Scaled = vectorFromP2ToP3 * t;
	//Add the scaled vector to the starting points to get a new point.
	//Remember, we need to add them because we have directions not positions.
	//We get positions by adding the directional vectors on the original positions
	BMath::Vec2 point1 = p1 + vectorFromP1ToP2Scaled;
	BMath::Vec2 point2 = p2 + vectorFromP2ToP3Scaled;
	//Return the two points
	toReturn.push_back(point1);
	toReturn.push_back(point2);
	return toReturn;
}
void BEngine::BezierCurveRecursive(std::vector<BMath::Vec2> points,
	float t,
	BMath::Vec2 & output) {
	if (points.size() == 3) {
		BMath::Vec2 toReturn = QuadraticBezierCurve(points[0], points[1], points[2], t);
		output.x = toReturn.x;
		output.y = toReturn.y;
	}
	else if (points.size() > 3) {
		std::vector<BMath::Vec2> run;
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
//############################ End lines and curves #####################
//######################## End interpolation #########################
//######################## String and sprite drawing ############################
void BEngine::DrawString(std::string string, int posX, int posY, int size, BColors::color_t color) {
	for (int pq = 0; pq < string.size(); pq++) {
		char c = string[pq];
		int drawingX = posX + pq * size * 0.6;
		int drawingY = posY;
		//The positon of the font in the sprite. Our font sprite encodes 
		//our characters in hexcadecimal.
		//i.e. 'A' is 0x41 and in sprite space, 'A' is located at 1,4
		int x = c & 0x0F;
		int y = c >> 4 & 0x0F;
		//How many pixels does every font covers - In this case 16x16
		int pixelsPerFont = 16; //16x16
		//I and J are real x and y in the image space
		int  i = x * pixelsPerFont;
		int j = y * pixelsPerFont;
		//Font width and height
		int fontHeight = size;
		int fontWidth = size;
		//Start at drawingX and drawingY
		for (int x1 = drawingX; x1 < drawingX + fontWidth; x1++) {
			for (int y1 = drawingY; y1 < drawingY + fontHeight; y1++) {
				//Normalize the x and y
				float normalizedX = (float)(x1 - drawingX) / fontWidth;
				float normalizedY = (float)(y1 - drawingY) / fontHeight;
				//Now we scale the i and j in respect to x and y. We are normalizing and mapping 
				//rendering space to font space
				int iToScale = normalizedX * pixelsPerFont;
				int jToScale = normalizedY * pixelsPerFont;

				//Then we get the color from the texture and set the pixel
				BColors::color_t colorFromTexture = BUtils::GetColorFromTexture((float)(i + iToScale) / fontsTexture.width, (float)(j + jToScale) / fontsTexture.height, &fontsTexture);
				if (colorFromTexture.alpha > 0) {
					SetBlendingMode(ALPHA);
					SetPixel(x1, y1, color);
					SetBlendingMode(NORMAL);
				}
			}
		}
	}
	
}
void BEngine::DrawString(const char * constString, int posX, int posY, int size, BColors::color_t color) {
	std::string string;
	string.append(constString);
	DrawString(string, posX, posY, size, color);
}
void BEngine::DrawString(std::string string, int posX, int posY, int size, int colorPacked) {
	DrawString(string, posX, posY, size, IntToColor(colorPacked));
}
void BEngine::DrawString(const char * constString, int posX, int posY, int size, int colorPacked) {
	DrawString(constString, posX, posY, size, IntToColor(colorPacked));
}
void BEngine::DrawVector(int posX, int posY,int size, BMath::Vec4 & vector, BColors::color_t color) {
	DrawString(std::to_string(vector.x), posX, posY, size, color);
	DrawString(std::to_string(vector.y), posX, posY + size, size, color);
	DrawString(std::to_string(vector.z), posX, posY + size * 2, size, color);
	DrawString(std::to_string(vector.w), posX, posY + size * 3, size, color);
}
void BEngine::DrawSprite(Sprite & sprite, BMath::Vec2 pos) {
	int width = sprite.width * sprite.scale;
	int height = sprite.height * sprite.scale;
	bool modeChanged = false;
	if (blendMode == NORMAL) { modeChanged = true; blendMode = ALPHA; }
	int startingX = pos.x - width / 2;
	int startingY = pos.y - height / 2;
	int endingX = pos.x + width / 2;
	int endingY = pos.y + height / 2;

	//TODO: We shouldnt be getting the pixel value from the texture
	//at every draw sprite call. I think we should attach some memory
	//to the sprite and everytime we want to draw the sprite, we copy
	//that memory to the sprite current pos.
	for (int x = startingX; x < endingX; x += pixelDimension) {
		for (int y = startingY; y < endingY; y += pixelDimension) {
			float normalizedX = 1.0f - ((float)(endingX - x) / width);
			float normalizedY = 1.0f - ((float)(endingY - y) / height);
			BColors::color_t color = BUtils::GetColorFromTexture(normalizedX, normalizedY, sprite.tex);
			if (sprite.tint) {
				float t = sprite.tintingPercentage;
				float t2 = 1 - t;
				color.red = color.red * t2 + sprite.tinting.red * t;
				color.green = color.green * t2 + sprite.tinting.green * t;
				color.blue = color.blue * t2 + sprite.tinting.blue * t;
			}
			SetPixel(x, y, color);
		}
	}
	if (modeChanged)blendMode = NORMAL;
}
//########################### End string drawing ######################
//######################## Input #############################
void BEngine::ProcessKeys() {
	//Note: This function sometimes can take upto 5ms even in release mode,
	//and may lead to drop in frame rate.  Maybe we should
	//only cater for less keys or run this function in a 
	//completely different thread.
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
	this->mouseInfoOld.x = mouseInfo.x;
	this->mouseInfoOld.y = mouseInfo.y;
	GetCursorPos(&this->mouseInfo);
	ScreenToClient(this->window, &this->mouseInfo);
	mouseDeltaX = mouseInfo.x - mouseInfoOld.x;
	mouseDeltaY = mouseInfo.y - mouseInfoOld.y;
}
//######################### end input #####################
//######################### Debug ######################
void BEngine::WriteTimingOutput() {
	NSDebug::WriteTimingDataOut(&this->timingData);
}
//###################### End debug #######################
//###################### Helper functions ################
BColors::color_t BEngine::IntToColor(int color) {
	BColors::color_t toReturn;
	toReturn.red = color >> 16 & 0xFF;
	toReturn.green = color >> 8 & 0xFF;
	toReturn.blue = color & 0xFF;
	return toReturn;
}
//###################### End helper functions
//####################### Sprites implementation
Sprite::Sprite(Texture * t) {
	this->tex = t;
	this->width = tex->width;
	this->height = tex->height;
}
Sprite::Sprite(Texture * t, int height, int width, float scale) {
	this->tex = t;
	this->height = height;
	this->width = width;
	this->scale = scale;
}
void Sprite::SetTinting(BColors::color_t color, float percentage) {
	this->tinting = color;
	this->tintingPercentage = percentage;
	this->tint = true;
}
void Sprite::ScaleSprite(float newScaleValue) { this->scale = newScaleValue; }
unsigned int Sprite::GetHeight() {
	return this->height * this->scale;
}
unsigned int Sprite::GetWidth() {
	return this->width * scale;
}
//################ end Sprites ########################

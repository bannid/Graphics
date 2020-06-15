#pragma once
#include <Windows.h>
#include <wingdi.h>
#include <gl/gl.h>
#include <vector>
#include <chrono>
#include <assert.h>
#include <cmath>
#include <strstream>
#include "Common.h"
#include "Maths.h"
#include "Debug.h"
#include "Mesh.h"
#include "lodepng.h"


class BEngine
{
public:
	//Constructors
	BEngine();
	//Destructor
	~BEngine();
	//Window creation and helpers
	HWND window;
	WNDCLASS windowClass = {};
	void GetDesktopResolution(int&, int&);
	bool InitOpenGl();
	void Win32UpdateWindow(HDC, RECT*, int, int, int, int);
	void Win32UpdateWindowOpenGL(HDC deviceContext, RECT* clientRect, int x, int y, int width, int height);
	void Win32ResizeDIBSection(int, int);
	bool Construct(int, int, int);
	bool Construct(int);
	bool Start();
	//Virtuals
	virtual bool OnUpdate(float) = 0;
	virtual bool OnCreate() = 0;
	virtual bool OnDestroy();
public:
	volatile bool running;
	POINT mouseInfo;
	POINT mouseInfoOld;
	float mouseDeltaX;
	float mouseDeltaY;
	//Debug
	std::map<std::string, std::vector<int>> timingData;
private:
	//Window construction
	LPCWSTR windowName;
	int windowHeight;
	int windowWidth;
	//Private struct to handle memory
	struct videoMemory_t {
		BITMAPINFO bitmapInfo;
		void *bitmapMemory;
		void *zBuffer;
		int bitmapWidth;
		int bitmapHeight;
		int bytesPerPixel = 4;
	};
	videoMemory_t screenInfo;
public:
	//Getters
	int GetScreenWidth();
	int GetScreenHeight();
	int GetPixelDimension();
	float GetZBuffer(int x, int y);
	//Returns the state of the key - key -> 0 to 0xff
	BInput::Key GetKey(unsigned int key);
	POINT GetMouseInfo();
	//Assets
	bool LoadTexturePNG(const char * fileName, Texture * output, bool loadAlpha);
	//Drawing routines
	//Draw line
	void DrawLine(int x1, int y1, int x2, int y2, BColors::color_t & color);
	void DrawLine(int x1, int y1, int x2, int y2, int color);
	void DrawString(std::string string, int posX, int posY, int size, BColors::color_t = { 0,0,0 });
	void DrawString(const char * constString, int posX, int posY, int size, BColors::color_t color = { 0,0,0 });
	void DrawString(std::string string, int x, int y, int size, int colorPacked = BColors::BLACK);
	void DrawString(const char * constString, int posX, int posY, int size, int colorPacked = BColors::BLACK);
	void DrawVector(int posX, int posY, int size,BMath::Vec4 & vector, BColors::color_t = { 255,255,255 });
	//Circle
	void DrawCircle(int x, int y, int radius, BColors::color_t & color);
	void DrawCircle(int x, int y, int radius, int colorPacked);
	void DrawCircle(BMath::Vec2 & pos, int radius, BColors::color_t & color);
	void DrawCircle(BMath::Vec2 & pos, int radius, int colorPacked);
	void FillCircle(int x, int y, int radius, BColors::color_t & color);
	void FillCircle(const BMath::Vec2 & pos, int radius, BColors::color_t & color);
	void FillCircle(int x, int y, int radius, int colorPacked);
	void FillCircle(const BMath::Vec2 & pos, int radius, int colorPacked);
	//Rectagle - TODO
	void DrawRectangle(int xTop, int yTop, int xBottom, int yBottom, BColors::color_t & color);
	void FillRectangle(int xTop, int yTop, int xBottom, int yBottom, BColors::color_t & color);
	void DrawRectangle(int xTop, int yTop, int xBottom, int yBottom, int colorPacked);
	void FillRectangle(int xTop, int yTop, int xBottom, int yBottom, int colorPacked);
	//Triangle 
	void FillTriangle(Triangle & t);
	void DrawTriangle(Triangle & t);
	void DrawTriangle(Vertex & one, Vertex & two, Vertex & three);
	//Sprite - Affine transformations cannot be applied
	void DrawSprite(Sprite & sprite, BMath::Vec2 pos);
	//Functions to draw bezier curve in screen space.
	void DrawBezierCurve(BMath::Vec2 p1, BMath::Vec2 cp, BMath::Vec2 p2, BColors::color_t & color);
	BMath::Vec2 QuadraticBezierCurve(BMath::Vec2 p1, BMath::Vec2 cp, BMath::Vec2 p2, float t);
	void BezierCurveRecursive(std::vector<BMath::Vec2> points, float t, BMath::Vec2 & ouput);
	//Routines to manipulate framebuffer
	void ClearScreen(BColors::color_t & color);
	void ClearScreenWithTexture(Texture * texture);
	void ClearZBuffer();
	void ClearScreen(int colorPacked);
	void SetPixel(int x, int y, BColors::color_t color);
	void SetPixel(int x, int y, int colorPacked);
	void SetZBuffer(int x, int y,float value);
	//Set blending mode - ALPHA or NORMAL
	void SetBlendingMode(BLENDING_MODE mode);
	//Helper functions
	BColors::color_t IntToColor(int color);
	//Input
	void ProcessKeys();
	//Debug
	void WriteTimingOutput();
	
private:
	//Private functions
	void SetPixelInternal(int x, int y, BColors::color_t & color);
	std::vector<BMath::Vec2>GetTwoLinearPointsFromThreePoints(BMath::Vec2 p1, BMath::Vec2 p2, BMath::Vec2 p3, float t);
private:
	//Timers to calculate elapsed time for OnUpdate
	std::chrono::steady_clock::time_point uct1;
	std::chrono::steady_clock::time_point uct2;
	BInput::Key keys[0xFF];
	int pixelDimension;
	std::vector<Texture> textures;
	Texture fontsTexture;
	BLENDING_MODE blendMode = NORMAL;
	//OpenGL
	GLuint blitTextureHandle;
};



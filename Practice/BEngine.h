#pragma once
#include <Windows.h>
#include <vector>
#include <chrono>
#include <assert.h>
#include <cmath>
#include "Common.h"
#include "Debug.h"
#include "lodepng.h"
#define TEXID int

struct Texture {
	unsigned char * data;
	unsigned int width;
	unsigned int height;
	unsigned int bytesPerPixel;
	TEXID id;
};
class BEngine
{
public:
	//Constructors
	BEngine();
	BEngine(LPCWSTR,
		int,
		int);
	//Destructor
	~BEngine();
	//Window creation and helpers
	HWND window;
	WNDCLASS windowClass = {};
	void GetDesktopResolution(int&, int&);
	void Win32UpdateWindow(HDC, RECT*, int, int, int, int);
	void Win32ResizeDIBSection(int, int);
	bool Construct(int,int,int);
	bool Construct();
	bool Start();
	//Virtuals
	virtual bool OnUpdate(float) = 0;
	virtual bool OnCreate() = 0;
public:
	bool running;
	POINT mouseInfo;
private:
	//Window construction
	LPCWSTR windowName;
	int windowHeight;
	int windowWidth;
	videoMemory_t screenInfo;

	//Rendering
public:
	//Setters
	void SetClearColor(color_t);
	void SetClearColor(int);
	void SetColor(color_t);
	void SetColor(int);
	//Getters
	int GetScreenWidth();
	int GetScreenHeight();
	int GetPixelDimension();
	NSInput::Key GetKey(unsigned int);
	color_t GetColor();
	color_t GetClearColor();
	unsigned int GetColorRGBPacked();
	unsigned int GetClearColorRGBPacked();
	POINT GetMouseInfo();
	float Lerp(float, float, float);
	NSMath2d::Vec2 Lerp(NSMath2d::Vec2, NSMath2d::Vec2, float);
	//Assets
	bool LoadTexturePNG(const char *, TEXID&,bool);
	color_t GetColorFromTexture(float, float, TEXID);
	//Drawing routines
	void DrawLine(int, int, int, int);
	void DrawBezierCurve(NSMath2d::Vec2,NSMath2d::Vec2,NSMath2d::Vec2);
	void DrawCircle(int, int, int);
	void DrawCircle(NSMath2d::Vec2 &, int);
	void DrawRectangle(int, int, int, int);
	void FillRectangle(int,int,int,int);
	void FillCircle(int,int,int);
	void FillCircle(const NSMath2d::Vec2 &, int);
	void SetPixel(int, int);
	void SetPixel(int, int, color_t);
	NSMath2d::Vec2 QuadraticBezierCurve(NSMath2d::Vec2, NSMath2d::Vec2, NSMath2d::Vec2, float);
	void BezierCurveRecursive(std::vector<NSMath2d::Vec2>, float, NSMath2d::Vec2&);
	void ClearScreen();
	//Input
	void ProcessKeys();
	//Debug
	void WriteTimingOutput();
private:
	//Private functions
	void SetPixelInternal(int x, int y, color_t);
	std::vector<NSMath2d::Vec2>GetTwoLinearPointsFromThreePoints(NSMath2d::Vec2 p1, NSMath2d::Vec2 p2, NSMath2d::Vec2 p3, float t);
private:
	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
	color_t color;
	color_t clearColor;
	unsigned int RGBPackedColor;
	unsigned int RGBPackedClearColor;
	NSInput::Key keys[0xFF];
	int pixelDimension;
	std::vector<Texture> textures;
	//Debug
	std::map<std::string, std::vector<int>> timingData;
};



#pragma once
#include <Windows.h>
#include <vector>
#include "Common.h"
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
	virtual bool OnUpdate() = 0;
public:
	bool running;
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
	NSInput::Key GetKey(unsigned int);
	color_t GetColor();
	color_t GetClearColor();
	unsigned int GetColorRGBPacked();
	unsigned int GetClearColorRGBPacked();
	POINT GetMouseInfo();
	float Lerp(float, float, float);
	NSMath2d::Vec2 Lerp(NSMath2d::Vec2, NSMath2d::Vec2, float);
	//Drawing routines
	void DrawLine(int, int, int, int);
	void DrawCircle(int, int, int);
	void DrawRectangle(int, int, int, int);
	void FillRectangle(int,int,int,int);
	void FillCircle(int,int,int);
	void SetPixel(int, int);
	NSMath2d::Vec2 QuadraticBezierCurve(NSMath2d::Vec2, NSMath2d::Vec2, NSMath2d::Vec2, float);
	void BezierCurveRecursive(std::vector<NSMath2d::Vec2>, float, NSMath2d::Vec2&);
	void ClearScreen();
	//Input
	void ProcessKeys();
private:
	//Private functions
	void SetPixelInternal(int x, int y);
	std::vector<NSMath2d::Vec2>GetTwoLinearPointsFromThreePoints(NSMath2d::Vec2 p1, NSMath2d::Vec2 p2, NSMath2d::Vec2 p3, float t);
private:
	color_t color;
	color_t clearColor;
	unsigned int RGBPackedColor;
	unsigned int RGBPackedClearColor;
	NSInput::Key keys[0xFF];
	int pixelDimension;
	POINT mouseInfo;
};



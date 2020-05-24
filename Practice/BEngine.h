#pragma once
#include <Windows.h>
#include <wingdi.h>
#include <gl/gl.h>
#include <vector>
#include <chrono>
#include <assert.h>
#include <cmath>
#include "Common.h"
#include "Debug.h"
#include "lodepng.h"

#define TEXID int
#define MILISECONDS_TO_SEC(VAL) VAL /= 1000.0f
#define FPS_60 16
#define FPS_30 32
struct Texture {
	unsigned char * data;
	unsigned int width;
	unsigned int height;
	unsigned int bytesPerPixel;
	TEXID id;
};
struct Sprite {
	float scale = 1;
	unsigned int height;
	unsigned int width;
	Texture * tex;
	NSMath2d::Vec2 pos;
	Sprite(Texture *, NSMath2d::Vec2 pos);
	Sprite(Texture *, NSMath2d::Vec2 pos, int height, int width, float scale);
	void ScaleSprite(float newScaleValue);
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
	void InitOpenGl();
	void Win32UpdateWindow(HDC, RECT*, int, int, int, int);
	void Win32UpdateWindowOpenGL(HDC deviceContext, RECT* clientRect, int x, int y, int width, int height);
	void Win32ResizeDIBSection(int, int);
	bool Construct(int, int, int);
	bool Construct(int);
	bool Start();
	//Virtuals
	virtual bool OnUpdate(float) = 0;
	virtual bool OnCreate() = 0;
public:
	bool running;
	POINT mouseInfo;
	//Debug
	std::map<std::string, std::vector<int>> timingData;
private:
	//Window construction
	LPCWSTR windowName;
	int windowHeight;
	int windowWidth;
	videoMemory_t screenInfo;

	//Rendering
public:
	//Getters
	int GetScreenWidth();
	int GetScreenHeight();
	int GetPixelDimension();
	//Returns the state of the key - key -> 0 to 0xff
	NSInput::Key GetKey(unsigned int key);
	POINT GetMouseInfo();
	float Lerp(float, float, float);
	NSMath2d::Vec2 Lerp(NSMath2d::Vec2 pointOne, NSMath2d::Vec2 pointTwo, float t);
	//Assets
	bool LoadTexturePNG(const char * fileName, TEXID& idOutput, bool loadAlpha);
	color_t GetColorFromTexture(float xNormalized, float yNormalized, TEXID textureId);
	Texture * GetTexture(TEXID tId);
	//Drawing routines
	void DrawLine(int x1, int y1, int x2, int y2, color_t & color);
	void DrawCircle(int x, int y, int radius, color_t & color);
	void DrawCircle(NSMath2d::Vec2 & pos, int radius, color_t & color);
	void FillCircle(int x, int y, int radius, color_t & color);
	void FillCircle(const NSMath2d::Vec2 & pos, int radius, color_t & color);
	void DrawRectangle(int xTop, int yTop, int xBottom, int yBottom, color_t & color);
	void FillRectangle(int xTop, int yTop, int xBottom, int yBottom, color_t & color);
	void DrawSprite(Sprite & sprite);
	void DrawBezierCurve(NSMath2d::Vec2 p1, NSMath2d::Vec2 cp, NSMath2d::Vec2 p2, color_t & color);
	NSMath2d::Vec2 QuadraticBezierCurve(NSMath2d::Vec2 p1, NSMath2d::Vec2 cp, NSMath2d::Vec2 p2, float t);
	void BezierCurveRecursive(std::vector<NSMath2d::Vec2> points, float t, NSMath2d::Vec2 & ouput);
	void ClearScreen(color_t & color);
	void SetPixel(int x, int y, color_t & color);
	//Input
	void ProcessKeys();
	//Debug
	void WriteTimingOutput();
private:
	//Private functions
	void SetPixelInternal(int x, int y, color_t & color);
	std::vector<NSMath2d::Vec2>GetTwoLinearPointsFromThreePoints(NSMath2d::Vec2 p1, NSMath2d::Vec2 p2, NSMath2d::Vec2 p3, float t);
private:
	//Timers to control frame rate
	std::chrono::steady_clock::time_point fct1;
	std::chrono::steady_clock::time_point fct2;
	//Timers to calculate elapsed time for OnUpdate
	std::chrono::steady_clock::time_point uct1;
	std::chrono::steady_clock::time_point uct2;
	NSInput::Key keys[0xFF];
	int pixelDimension;
	std::vector<Texture> textures;
	GLuint blitTextureHandle;
};



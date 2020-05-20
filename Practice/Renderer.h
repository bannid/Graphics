#pragma once
#include "Common.h"
#include <WinUser.h>
#define RGBC(r,g,b) (((r << 8) | g)<<8) | b
namespace NSInput {
	struct Key {
		bool keyDown = false;
		bool keyReleased = false;
		bool keyHeld = false;
	};
}
namespace NSMath2d {
	struct Vec2 {
		float x, y;
		Vec2(float x, float y) :x(x), y(y) {
		}
		float Magnitude() {
			return std::sqrtf(x * x + y * y);
		}
		float operator*(Vec2 & that) {
			return x * that.x + y * that.y;
		}
		Vec2 operator-(Vec2 & that) {
			return Vec2(x - that.x, y - that.y);
		}
		Vec2 operator+(Vec2 & that) {
			return Vec2(x + that.x, y + that.y);
		}
		Vec2 operator*(float scalar) {
			return Vec2(x * scalar, y * scalar);
		}
		void Scale(float scalar) {
			x *= scalar;
			y *= scalar;
		}
		void Normalize() {
			float magnitude = this->Magnitude();
			x /= magnitude;
			y /= magnitude;
		}
		Vec2 Normalized() {
			float magnitude = this->Magnitude();
			return Vec2(x / magnitude, y / magnitude);
		}
	};
}
namespace NSShapes {
	struct Triangle {
		NSMath2d::Vec2 vertice1;
		NSMath2d::Vec2 vertice2;
		NSMath2d::Vec2 vertice3;
		Triangle(NSMath2d::Vec2 vertice1,
			NSMath2d::Vec2 vertice2,
			NSMath2d::Vec2 vertice3) :vertice1(vertice1), vertice2(vertice2), vertice3(vertice3) {

		}
	};
	struct Rectangle {
		//We just need the upper left and lower right edge
		//to draw rectangles
		NSMath2d::Vec2 vertice1;
		NSMath2d::Vec2 vertice2;
		Rectangle(NSMath2d::Vec2 vertice1,
			NSMath2d::Vec2 vertice2) :vertice1(vertice1), vertice2(vertice2) {

		}
	};
	struct Circle {
		float radius;
		Circle(float radius) :radius(radius) {

		}
	};
	struct Line {
		NSMath2d::Vec2 origin;
		NSMath2d::Vec2 destination;
		Line(NSMath2d::Vec2 origin,
			NSMath2d::Vec2 destination) :origin(origin), destination(destination) {

		}
	};
}
namespace NSRender {
	enum COLORS {
		BLACK = 0,
		RED = RGBC(255, 0, 0),
		BLUE = RGBC(0, 0, 255),
		GREEN = RGBC(0, 255, 0),
		WHITE = RGBC(255, 255, 255),
		YELLOW = RGBC(255, 255, 0)
	};
	class Renderer
	{
	public:
		Renderer(videoMemory_t*, int pixelDim);
		~Renderer();
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
		float Lerp(float , float, float);
		NSMath2d::Vec2 Lerp(NSMath2d::Vec2, NSMath2d::Vec2, float);
		//Drawing routines
		void DrawLine(int, int, int, int);
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
		videoMemory_t * videoMemoryInfo;
		color_t color;
		color_t clearColor;
		unsigned int RGBPackedColor;
		unsigned int RGBPackedClearColor;
		NSInput::Key keys[0xFF];
		int pixelDimension;
		POINT mouseInfo;
	};
}

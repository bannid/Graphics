#pragma once
#include "Common.h"
#define RGBC(r,g,b) (((r << 8) | g)<<8) | b
namespace math2d {
	struct Vec2 {
		float x, y;
		Vec2(float x, float y):x(x),y(y) {
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
namespace shapes {
	struct Triangle {
		math2d::Vec2 vertice1;
		math2d::Vec2 vertice2;
		math2d::Vec2 vertice3;
		Triangle(math2d::Vec2 vertice1,
			math2d::Vec2 vertice2,
			math2d::Vec2 vertice3):vertice1(vertice1),vertice2(vertice2),vertice3(vertice3) {

		}
	};
	struct Rectangle {
		//We just need the upper left and lower right edge
		//to draw rectangles
		math2d::Vec2 vertice1;
		math2d::Vec2 vertice2;
		Rectangle(math2d::Vec2 vertice1,
			math2d::Vec2 vertice2) :vertice1(vertice1), vertice2(vertice2) {

		}
	};
	struct Circle {
		float radius;
		Circle(float radius) :radius(radius) {

		}
	};
	struct Line {
		math2d::Vec2 origin;
		math2d::Vec2 destination;
		Line(math2d::Vec2 origin,
			math2d::Vec2 destination) :origin(origin), destination(destination) {

		}
	};
}
namespace rr {
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
		Renderer(videoMemory_t*,int pixelDim);
		~Renderer();
		void SetPixel(int x, int y);
		void ClearScreen();
		void SetClearColor(color_t color);
		void SetClearColor(int rgb);
		void SetColor(color_t color);
		void SetColor(int rgb);

		//Drawing routines
		void DrawLine(int, int, int, int);
		math2d::Vec2 QuadraticBezierCurve(math2d::Vec2, math2d::Vec2, math2d::Vec2, float);
		void BezierCurveRecursive(std::vector<math2d::Vec2>, float, math2d::Vec2&);
	
	private:
		//Private functions
		void SetPixelInternal(int x, int y);
		std::vector<math2d::Vec2>GetTwoLinearPointsFromThreePoints(math2d::Vec2 p1, math2d::Vec2 p2, math2d::Vec2 p3, float t);
	private:
		videoMemory_t * videoMemoryInfo;
		color_t color;
		color_t clearColor;
		int pixelDimension;
	};
}

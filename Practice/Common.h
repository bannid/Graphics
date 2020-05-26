#pragma once
#include <Windows.h>
#define RGBC(r,g,b) (((r << 8) | g)<<8) | b
struct videoMemory_t {
	BITMAPINFO bitmapInfo;
	void *bitmapMemory;
	int bitmapWidth;
	int bitmapHeight;
	int bytesPerPixel = 4;
};
struct color_t {
	int red;
	int green;
	int blue;
	int alpha;
	color_t() {
		this->red = 0;
		this->green = 0; 
		this->blue = 0;
		this->alpha = 0xFF;
	}
	color_t(int red,int green, int blue) {
		this->green = green;
		this->blue = blue;
		this->red = red;
		this->alpha = 0xFF;
	}
	color_t(int red, int green, int blue, int alpha) {
		this->green = green;
		this->blue = blue;
		this->red = red;
		this->alpha = alpha;
	}
};
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
		Vec2(int x, int y) :x(x), y(y) {

		}
		Vec2() {
			x = 0;
			y = 0;
		}
		float Magnitude() {
			return std::sqrtf(x * x + y * y);
		}
		void Add(Vec2 that) {
			this->x += that.x;
			this->y += that.y;
		}
		void Subtract(Vec2 & that) {
			this->x -= that.x;
			this->y -= that.y;
		}
		float operator*(Vec2 & that) {
			return x * that.x + y * that.y;
		}
		Vec2 operator-(Vec2 & that) {
			return Vec2(x - that.x, y - that.y);
		}
		Vec2 operator+(Vec2 that) {
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
namespace NSColors {
	enum COLORS {
		BLACK = 0,
		RED = RGBC(255, 0, 0),
		BLUE = RGBC(0, 0, 255),
		GREEN = RGBC(0, 255, 0),
		WHITE = RGBC(255, 255, 255),
		YELLOW = RGBC(255, 255, 0)
	};
}
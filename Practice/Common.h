#pragma once
#include <Windows.h>
#include "Maths.h"
#define RGBC(r,g,b) (((r << 8) | g)<<8) | b
struct videoMemory_t {
	BITMAPINFO bitmapInfo;
	void *bitmapMemory;
	int bitmapWidth;
	int bitmapHeight;
	int bytesPerPixel = 4;
};

namespace BInput {
	struct Key {
		bool keyDown = false;
		bool keyReleased = false;
		bool keyHeld = false;
	};
}
namespace BColors {
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
		color_t(int red, int green, int blue) {
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
	enum COLORS {
		BLACK = 0,
		RED = RGBC(255, 0, 0),
		BLUE = RGBC(0, 0, 255),
		GREEN = RGBC(0, 255, 0),
		WHITE = RGBC(255, 255, 255),
		YELLOW = RGBC(255, 255, 0)
	};
}

struct Vertex {
	BMath::Vec4 vector;
	BMath::Vec2 uv;
	BColors::color_t color;
	BMath::Vec4 Normal;
};
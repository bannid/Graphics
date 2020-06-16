#pragma once
#include <Windows.h>
#include "Maths.h"
#define RGBC(r,g,b) (((r << 8) | g)<<8) | b
#define M_PI 3.1415926535
#define RAD_TO_DEGREE(x) 180.0f/M_PI * x
#define DEGREE_TO_RAD(x) M_PI/180.0f * x
#define MILISECONDS_TO_SEC(VAL) VAL /= 1000.0f
#define FPS_60 16
#define FPS_30 32
struct BKey {
	bool keyDown = false;
	bool keyReleased = false;
	bool keyHeld = false;
};
struct BColor {
	int red;
	int green;
	int blue;
	int alpha;
	BColor() {
		this->red = 0;
		this->green = 0;
		this->blue = 0;
		this->alpha = 0xFF;
	}
	BColor(int red, int green, int blue) {
		this->green = green;
		this->blue = blue;
		this->red = red;
		this->alpha = 0xFF;
	}
	BColor(int red, int green, int blue, int alpha) {
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
	GRAY = RGBC(211, 211, 211),
	YELLOW = RGBC(255, 255, 0),
	MAROON = RGBC(128, 0, 0)
};
struct Vertex {
	BMath::Vec4 vector;
	BColor color;
	BMath::Vec4 normal;
	BMath::Vec4 uv;
};
struct Triangle {
	Vertex vertices[3];
	float Area();
};
struct Texture {
	unsigned char * data;
	unsigned int width;
	unsigned int height;
	unsigned int bytesPerPixel;
};
struct Sprite {
	float scale = 1;
	unsigned int height;
	unsigned int width;
	BColor tinting;
	bool tint = false;
	float tintingPercentage;
	Texture * tex;
	Sprite(Texture *);
	Sprite(Texture *, int height, int width, float scale);
	void ScaleSprite(float newScaleValue);
	void SetTinting(BColor color, float percentage);
	unsigned int GetHeight();
	unsigned int GetWidth();

};
enum BLENDING_MODE { ALPHA, NORMAL };

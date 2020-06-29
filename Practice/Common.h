#pragma once
#include <Windows.h>
#include <assert.h>
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
	float red;
	float green;
	float blue;
	float alpha;
	BColor() {
		this->red = 0;
		this->green = 0;
		this->blue = 0;
		this->alpha = 1.0f;
	}
	BColor(float red, float green, float blue) {
		this->green = green;
		this->blue = blue;
		this->red = red;
		this->alpha = 1.0f;
	}
	BColor(float red, float green, float blue, float alpha) {
		this->green = green;
		this->blue = blue;
		this->red = red;
		this->alpha = alpha;
	}
};

struct Vertex {
	BMath::Vec4 vector;
	BColor color = { 1.0f,1.0f,1.0f };
	BMath::Vec4 normal;
	BMath::Vec4 uv;
	float zInverse;
};
struct Triangle {
	Vertex one;
	Vertex two;
	Vertex three;
	Triangle(Vertex one, Vertex two, Vertex three);
	BColor color = { 1.0f,1.0f,1.0f };
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
struct Ray {
	BMath::Vec4 direction;
	BMath::Vec4 origin;
};
struct Line {
	BMath::Vec4 direction;
	BMath::Vec4 origin;
};
struct LightDirectional {
	BMath::Vec4 direction;
	BColor color = { 1.0f,1.0f,0.0f };
	float intensity;
};
struct LightPostitional {
	BMath::Vec4 position;
	BColor color;
	float intensity;
};
struct Plane {
	//The plane equation is - normal, and known point on the plane 
	BMath::Vec4 normal;
	BMath::Vec4 pointOnPlane;
	Plane(BMath::Vec4 normal, BMath::Vec4 pointOnPlane);
	float IntersectWithRay(Ray ray);
	float IntersectWithLine(Line line);
};
enum BLENDING_MODE { ALPHA, NORMAL };
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
enum CULL {
	NO_CULL,
	FRONT_CULL,
	BACK_CULL
};

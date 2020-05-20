#include<iostream>
#include<vector>
#include<assert.h>

#include "Renderer.h"
#include "Common.h"
namespace NSRender {
	Renderer::Renderer(videoMemory_t * videoMemory, int pixelDim)
	{
		this->videoMemoryInfo = videoMemory;
		SetClearColor(NSRender::WHITE);
		this->pixelDimension = pixelDim;
	}

	Renderer::~Renderer()
	{

	}

	void Renderer::SetColor(color_t color) {
		this->color = color;
	}
	int Renderer::GetScreenHeight() {
		return this->videoMemoryInfo->bitmapHeight;
	}
	int Renderer::GetScreenWidth() {
		return this->videoMemoryInfo->bitmapWidth;
	}
	color_t Renderer::GetClearColor() {
		return this->clearColor;
	}
	color_t Renderer::GetColor() {
		return this->color;
	}
	POINT Renderer::GetMouseInfo() {
		return this->mouseInfo;
	}
	unsigned int Renderer::GetColorRGBPacked() {
		return this->RGBPackedColor;
	}
	unsigned int Renderer::GetClearColorRGBPacked() {
		return this->RGBPackedClearColor;
	}
	void Renderer::SetColor(int rgb) {
		this->RGBPackedColor = rgb;
		int red = rgb >> 16 & 0xFF;
		int green = rgb >> 8 & 0xFF;
		int blue = rgb & 0xFF;
		this->color.red = red;
		this->color.blue = blue;
		this->color.green = green;
	}
	void Renderer::SetClearColor(color_t color) {
		this->clearColor = color;
	}
	float Renderer::Lerp(float a, float b, float t){
		return ((1 - t) * a) + (t * b);
	}
	void Renderer::SetClearColor(int rgb) {
		this->RGBPackedClearColor = rgb;
		int red = rgb >> 16 & 0xFF;
		int green = rgb >> 8 & 0xFF;
		int blue = rgb & 0xFF;
		this->clearColor.red = red;
		this->clearColor.blue = blue;
		this->clearColor.green = green;
	}
	void Renderer::SetPixelInternal(int x, int y) {
		if (x < 0 || x >= this->videoMemoryInfo->bitmapWidth || y < 0 || y >= this->videoMemoryInfo->bitmapHeight) {
			return;
		}
		int memorySize = this->videoMemoryInfo->bitmapHeight * this->videoMemoryInfo->bitmapWidth;
		unsigned int * pixel = (unsigned int *)this->videoMemoryInfo->bitmapMemory;
		unsigned int toPlus = x + this->videoMemoryInfo->bitmapWidth * y;
		pixel += toPlus;
		*pixel = (((this->color.red << 8) | this->color.green) << 8) | this->color.blue;
	}
	void Renderer::SetPixel(int x, int y) {
		for (int i = x; i < x + this->pixelDimension; i++) {
			for (int k = y; k < y + this->pixelDimension; k++) {
				SetPixelInternal(i, k);
			}
		}
	}
	void Renderer::ClearScreen() {
		unsigned int memorySize = this->videoMemoryInfo->bitmapWidth * this->videoMemoryInfo->bitmapHeight;
		unsigned int * pixel = (unsigned int *)this->videoMemoryInfo->bitmapMemory;
		for (unsigned int i = 0; i < memorySize; i++) {
			*pixel = (((this->clearColor.red << 8) | this->clearColor.green) << 8) | this->clearColor.blue;
			pixel++;
		}
	}

	void Renderer::DrawLine(int x1, int y1, int x2, int y2) {
		bool steep = false;
		if (std::abs(y2 - y1) > std::abs(x2 - x1)) {
			steep = true;
			std::swap(x1, y1);
			std::swap(x2, y2);
		}
		if (x1 > x2) {
			std::swap(x2, x1);
			std::swap(y2, y1);
		}
		float dy = std::abs(y2 - y1);
		float dx = std::abs(x2 - x1);
		float slope = dy / dx;
		float delta = 0.0;
		unsigned int y = y1;
		for (int x = x1; x < x2; x++) {
			if (!steep) {
				SetPixel(x, y);
			}
			else {
				SetPixel(y, x);
			}
			delta += slope;
			if (delta > 0.5) {
				delta -= 1;
				y += (y2 - y1) > 0 ? 1 : -1;
			}
		}
	}
	NSMath2d::Vec2 Renderer::QuadraticBezierCurve(NSMath2d::Vec2 p1,
		NSMath2d::Vec2 cp,
		NSMath2d::Vec2 p2,
		float t) {
		auto s = GetTwoLinearPointsFromThreePoints(p1, cp, p2, t);
		NSMath2d::Vec2 finalVector = s[1] - s[0];
		NSMath2d::Vec2 finalVectorScaled = finalVector * t;
		return s[0] + finalVectorScaled;
	}
	//Helper function to draw Bezier Curve.  Given three points, we get two points back
	//in respect the value of t
	std::vector<NSMath2d::Vec2> Renderer::GetTwoLinearPointsFromThreePoints(NSMath2d::Vec2 p1, NSMath2d::Vec2 p2, NSMath2d::Vec2 p3, float t) {
		std::vector<NSMath2d::Vec2> toReturn;
		//Get first vector
		NSMath2d::Vec2 vectorFromP1ToP2 = p2 - p1;
		//Get second vector
		NSMath2d::Vec2 vectorFromP2ToP3 = p3 - p2;
		//Scale both of the vectors in respect to t
		NSMath2d::Vec2 vectorFromP1ToP2Scaled = vectorFromP1ToP2 * t;
		NSMath2d::Vec2 vectorFromP2ToP3Scaled = vectorFromP2ToP3 * t;
		//Add the scaled vector to the starting points to get a new point.
		//Remember, we need to add them because we have directions not positions.
		//We get positions by adding the directional vectors on the original positions
		NSMath2d::Vec2 point1 = p1 + vectorFromP1ToP2Scaled;
		NSMath2d::Vec2 point2 = p2 + vectorFromP2ToP3Scaled;
		//Return the two points
		toReturn.push_back(point1);
		toReturn.push_back(point2);
		return toReturn;
	}
	void Renderer::BezierCurveRecursive(std::vector<NSMath2d::Vec2> points,
		float t,
		NSMath2d::Vec2 & output) {
		if (points.size() == 3) {
			NSMath2d::Vec2 toReturn = QuadraticBezierCurve(points[0], points[1], points[2], t);
			output.x = toReturn.x;
			output.y = toReturn.y;
		}
		else if (points.size() > 3) {
			std::vector<NSMath2d::Vec2> run;
			for (int i = 0; i < points.size() - 2; i++) {
				auto temp = GetTwoLinearPointsFromThreePoints(points[i], points[i + 1], points[i + 2], t);
				if (i == 0) {
					for (int i = 0; i < temp.size(); i++) {
						run.push_back(temp[i]);
					}
				}
				else {
					for (int i = 1; i < temp.size(); i++) {
						run.push_back(temp[i]);
					}
				}
			}
			BezierCurveRecursive(run, t, output);
		}
	}
	NSMath2d::Vec2 Renderer::Lerp(NSMath2d::Vec2 a, NSMath2d::Vec2 b, float t) {
		NSMath2d::Vec2 vectorFromAToB = b - a;
		NSMath2d::Vec2 vectorFromAToBScaled = vectorFromAToB * t;
		return a + vectorFromAToBScaled;
	}
	void Renderer::ProcessKeys() {
		for (int key = 0; key < 0xff; key++) {
			if (!this->keys[key].keyDown) {
				this->keys[key].keyDown = 0x1 & (GetAsyncKeyState(key) >> 15);
				this->keys[key].keyReleased = false;
			}
			else {
				this->keys[key].keyDown = (0x1 & (GetAsyncKeyState(key) >> 15));
				if (this->keys[key].keyDown) {
					this->keys[key].keyHeld = true;
					this->keys[key].keyReleased = false;
				}
				else {
					this->keys[key].keyHeld = false;
					this->keys[key].keyReleased = true;
				}
			}
		}
		GetCursorPos(&this->mouseInfo);
	}
	NSInput::Key Renderer::GetKey(unsigned int key) {
		assert(key > 0 && key<0xFF);
		return this->keys[key];
	}
}
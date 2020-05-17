#include<iostream>
#include "Renderer.h"
#include "Common.h"

Renderer::Renderer(videoMemory_t * videoMemory)
{
	this->videoMemory = videoMemory;
	this->clearColor.red = 255;
	this->clearColor.green = 255;
	this->clearColor.blue = 255;
	this->valid = true;
}

Renderer::Renderer() {
	this->clearColor.red = 255;
	this->clearColor.green = 255;
	this->clearColor.blue = 255;
	this->valid = false;
}
void Renderer::SetVideoMemory(videoMemory_t * videoMemory) {
	this->videoMemory = videoMemory;
	this->valid = true;
}
Renderer::~Renderer()
{

}

void Renderer::SetColor(color_t color) {
	this->color = color;
}

void Renderer::SetPixel(int x, int y) {
	if (x < 0 || x > this->videoMemory->bitmapWidth || y < 0 || y > this->videoMemory->bitmapHeight) {
		return;
	}
	int memorySize = this->videoMemory->bitmapHeight * this->videoMemory->bitmapWidth;
	unsigned int * pixel = (unsigned int *)this->videoMemory->bitmapMemory;
	unsigned int toPlus = x + this->videoMemory->bitmapWidth * y;
	pixel += toPlus;
	*pixel = (((this->color.red << 8) | this->color.green) << 8) | this->color.blue;
}

void Renderer::ClearScreen() {
	unsigned int memorySize = this->videoMemory->bitmapWidth * this->videoMemory->bitmapHeight;
	unsigned int * pixel = (unsigned int *)this->videoMemory->bitmapMemory;
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

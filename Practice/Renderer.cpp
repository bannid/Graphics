#include<iostream>
#include "Renderer.h"
#include "Common.h"
namespace rr {
	Renderer::Renderer(videoMemory_t * videoMemory, int pixelDim)
	{
		this->videoMemoryInfo = videoMemory;
		this->clearColor.red = 255;
		this->clearColor.green = 255;
		this->clearColor.blue = 255;
		this->valid = true;
		this->pixelDimension = pixelDim;
	}

	Renderer::~Renderer()
	{

	}

	void Renderer::SetColor(color_t color) {
		this->color = color;
	}
	void Renderer::SetColor(int rgb) {
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
	void Renderer::SetClearColor(int rgb) {
		int red = rgb >> 16 & 0xFF;
		int green = rgb >> 8 & 0xFF;
		int blue = rgb & 0xFF;
		this->clearColor.red = red;
		this->clearColor.blue = blue;
		this->clearColor.green = green;
	}
	void Renderer::SetPixelInternal(int x, int y) {
		if (x < 0 || x > this->videoMemoryInfo->bitmapWidth || y < 0 || y > this->videoMemoryInfo->bitmapHeight) {
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
}
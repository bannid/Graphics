#pragma once
#include <Windows.h>
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
	color_t() {
		this->red = 0;
		this->green = 0; 
		this->blue = 0;
	}
	color_t(int red,int green, int blue) {
		this->green = green;
		this->blue = blue;
		this->red = red;
	}
};
#pragma once
#include "Common.h"
class Renderer
{
public:
	Renderer(videoMemory_t*);
	Renderer();
	~Renderer();
	void SetPixel(int x, int y);
	void ClearScreen();
	void DrawLine(int, int, int, int);
	void SetColor(color_t color);
	void SetVideoMemory(videoMemory_t * videoMemory);
private:
	videoMemory_t * videoMemory;
	color_t color;
	color_t clearColor;
	bool valid;
};


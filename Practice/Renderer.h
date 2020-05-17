#pragma once
#include "Common.h"
#define RGB(r,g,b) (((r << 8) | g)<<8) | b
namespace rr {
	class Renderer
	{
	public:
		Renderer(videoMemory_t*,int pixelDim);
		~Renderer();
		void SetPixel(int x, int y);
		void ClearScreen();
		void SetClearColor(color_t color);
		void SetClearColor(int rgb);
		void DrawLine(int, int, int, int);
		void SetColor(color_t color);
		void SetColor(int rgb);
	private:
		void SetPixelInternal(int x, int y);
	private:
		videoMemory_t * videoMemoryInfo;
		color_t color;
		color_t clearColor;
		bool valid;
		int pixelDimension;
	};
	enum COLORS {
		BLACK = 0,
		RED = RGB(255,0,0),
		BLUE = RGB(0,0,255),
		GREEN = RGB(0,255,0),
		WHITE = RGB(255,255,255),
		YELLOW = RGB(255,255,0)
	};
}

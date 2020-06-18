#pragma once
#include "Common.h"
#include "stb_image.h"

namespace BUtils {
	bool LoadTexturePNG(const char * fileName, Texture * texture);
	BColor GetColorFromTexture(float normalizedX, float normalizedY, Texture * texture);
	float Min(float a, float b);
	float Max(float a, float b);

}
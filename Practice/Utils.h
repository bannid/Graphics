#pragma once
#include "Common.h"
namespace BUtils {
	BColors::color_t GetColorFromTexture(float normalizedX, float normalizedY, Texture * texture);
	float Min(float a, float b);
	float Max(float a, float b);

}
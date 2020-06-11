#include "Utils.h"

BColors::color_t BUtils::GetColorFromTexture(float normalizedX, float normalizedY, Texture * texture) {
	if (normalizedX < 0 || normalizedX > 1 || normalizedY < 0 || normalizedY > 1) {
		return { 0,0,0,0 };
	}
	BColors::color_t color;

	int width = texture->width;
	int height = texture->height;
	int mappedX = normalizedX * width;
	int mappedY = normalizedY * height;
	if (mappedX != 0) mappedX--;
	if (mappedY != 0) mappedY--;
	unsigned int * intData = (unsigned int*)texture->data;
	int value = intData[mappedX + mappedY * width];
	color.alpha = value >> 24 & 0xFF;
	color.blue = value >> 16 & 0xFF;
	color.green = value >> 8 & 0xFF;
	color.red = value & 0xFF;
	return color;
}
float BUtils::Min(float a, float b) {
	return a < b ? a : b;
}
float BUtils::Max(float a, float b) {
	return a > b ? a : b;
}

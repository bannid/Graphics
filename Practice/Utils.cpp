#include "Utils.h"
#include "lodepng.h"

BColor BUtils::GetColorFromTexture(float normalizedX, float normalizedY, Texture * texture) {
	if (normalizedX < 0 || normalizedX > 1 || normalizedY < 0 || normalizedY > 1) {
		return { 0,0,0,0 };
	}
	BColor color;

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
bool BUtils::LoadTexturePNG(const char * fileName, Texture * output) {
	bool success = lodepng_decode32_file(&output->data,
		&output->width,
		&output->height,
		fileName) == 0;
	return success;
}
float BUtils::Min(float a, float b) {
	return a < b ? a : b;
}
float BUtils::Max(float a, float b) {
	return a > b ? a : b;
}

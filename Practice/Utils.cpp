#include "Utils.h"

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
	color.alpha = (float)(value >> 24 & 0xFF)/255.0f;
	color.blue = (float)(value >> 16 & 0xFF)/255.0f;
	color.green = (float)(value >> 8 & 0xFF)/255.0f;
	color.red = (float)(value & 0xFF)/255.0f;
	return color;
}
bool BUtils::LoadTexturePNG(const char * fileName, Texture * output) {
	output->data = stbi_load(fileName, (int*)&output->width, (int*)&output->height, (int*)&output->bytesPerPixel, 4);
	return output->data != NULL;
}
float BUtils::Min(float a, float b) {
	return a < b ? a : b;
}
float BUtils::Max(float a, float b) {
	return a > b ? a : b;
}

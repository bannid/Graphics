#include "Common.h"
//####################### Sprites implementation
Sprite::Sprite(Texture * t) {
	this->tex = t;
	this->width = tex->width;
	this->height = tex->height;
}
Sprite::Sprite(Texture * t, int height, int width, float scale) {
	this->tex = t;
	this->height = height;
	this->width = width;
	this->scale = scale;
}
void Sprite::SetTinting(BColor color, float percentage) {
	this->tinting = color;
	this->tintingPercentage = percentage;
	this->tint = true;
}
void Sprite::ScaleSprite(float newScaleValue) { this->scale = newScaleValue; }
unsigned int Sprite::GetHeight() {
	return this->height * this->scale;
}
unsigned int Sprite::GetWidth() {
	return this->width * scale;
}
//################ end Sprites ########################
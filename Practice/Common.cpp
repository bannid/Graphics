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

Plane::Plane(BMath::Vec4 normal, BMath::Vec4 pointOnPlane) {
	this->normal = normal;
	this->pointOnPlane = pointOnPlane;
}

float Plane::IntersectWithLine(Line line) {
	/*
		This function will give us the t value which we can back track
		to find the point of intersection.
		Equation to find T is as following:
			t = (-N . O + D) / N . Di
			where
				* t is what we are looking for
				* N is the normal of the plane
				* O is the origin of line
				* D is the distance of the plane from origin
				* Di is the direction of the line segment
*/
	float d = this->normal * this->pointOnPlane;
	float t = ((-(normal * line.origin)) + d) / (normal * line.direction);
	return t;
}

Triangle::Triangle(Vertex one, Vertex two, Vertex three) {
	this->one = one;
	this->two = two;
	this->three = three;
}
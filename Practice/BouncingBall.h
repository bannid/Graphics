#pragma once
#include <cmath>
#include "BEngine.h"
#include "Common.h"

class BouncingBall : public BEngine {
	NSMath2d::Vec2 pos;
	int radius;
	NSMath2d::Vec2 velocity;
	float angle = 0;
	virtual bool OnCreate() {
		SetClearColor(NSColors::BLACK);
		SetColor(NSColors::BLUE);
		pos.x = GetScreenWidth() / 2;
		pos.y = GetScreenHeight() / 2;
		velocity.x = 200.0f;
		velocity.y = 200.0f;
		radius = 0.1 * GetScreenWidth();
		angle = 0;
		return true;
	}

	virtual bool OnUpdate(float elapsedTime) override {
		ClearScreen();
		FillCircle(pos.x, pos.y, radius);
		NSMath2d::Vec2 velocityScaled = velocity * elapsedTime;
		pos.Add(velocityScaled);
		angle += 0.1;
		if (pos.x + radius >= GetScreenWidth() ||
			pos.x - radius<= 0) {
			velocity.x *= -1;
		}
		if (pos.y + radius >= GetScreenHeight() ||
			pos.y - radius <= 0) {
			velocity.y *= -1;
		}
		return true;
	}
};
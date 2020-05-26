#pragma once
#include <vector>
#include "BEngine.h"
#include "Common.h"
class DrawLines : public BEngine {
	std::vector<NSMath2d::Vec2> points;
	color_t black = { 0,0,0 };
	color_t white = { 255,255,255,125 };
	virtual bool OnUpdate(float elapsedTime) override{
		ClearScreen(black);
		auto mouseInfo = GetMouseInfo();
		if (GetKey(VK_LBUTTON).keyReleased) {
			NSMath2d::Vec2 point(mouseInfo.x, mouseInfo.y);
			points.push_back(point);
		}
		int pointsSize = points.size();
		for (int i = 0; i < pointsSize - 1; i++) {
			DrawLine(points[i].x, points[i].y, points[i + 1].x, points[i + 1].y, white);
		}
		if (pointsSize > 0) {
			DrawLine(points[pointsSize - 1].x, points[pointsSize - 1].y,
				mouseInfo.x, mouseInfo.y,white);
		}
		if (GetKey(0x41).keyReleased) {
			points.clear();
		}
		return true;
	}
	virtual bool OnCreate() override {

		return true;
	}
};
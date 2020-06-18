#pragma once
#include <vector>
#include "BEngine.h"
class DrawLines : public BEngine {
	std::vector<BMath::Vec2> points;
	BColor black = { 0,0,0 };
	BColor white = { 1.0f,1.0f,1.0f,0.5f };
	virtual bool OnUpdate(float elapsedTime) override{
		ClearScreen(black);
		auto mouseInfo = GetMouseInfo();
		if (GetKey(VK_LBUTTON).keyReleased) {
			BMath::Vec2 point(mouseInfo.x, mouseInfo.y);
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
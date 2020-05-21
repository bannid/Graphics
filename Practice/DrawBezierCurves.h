#pragma once
#include <vector>
#include "BEngine.h"
#include "Common.h"
struct Bezier {
	NSMath2d::Vec2 p1;
	NSMath2d::Vec2 cp;
	NSMath2d::Vec2 p2;
};
class BezierCurves : public BEngine {
	std::vector<Bezier> bezierCurves;
	NSMath2d::Vec2 * selector = nullptr;
	virtual bool OnCreate() override {
		SetClearColor(NSColors::BLACK);
		SetColor(NSColors::WHITE);
		Bezier b;
		b.p1 = NSMath2d::Vec2(10, GetScreenHeight() / 2);
		b.p2 = NSMath2d::Vec2(GetScreenWidth() - 50, GetScreenHeight() / 2);
		b.cp = NSMath2d::Vec2(GetScreenWidth() / 2, 20);
		bezierCurves.push_back(b);
		return true;
	}
	virtual bool OnUpdate(float elapsedTime) override {
		ClearScreen();
		for (auto b = bezierCurves.begin(); b != bezierCurves.end(); b++) {
			DrawBezierCurve(b->p1, b->cp, b->p2);
			DrawCircle(b->p1, 10);
			DrawCircle(b->p2, 10);
			DrawCircle(b->cp, 10);
			DrawCircle(mouseInfo.x, mouseInfo.y, 5);
			if (GetKey(VK_LBUTTON).keyDown &&
				selector == nullptr) {
				NSMath2d::Vec2 coords = { mouseInfo.x,mouseInfo.y };
				int distanceFromP1 = abs((b->p1 - coords).Magnitude());
				int distanceFromP2 = abs((b->p2 - coords).Magnitude());
				int distanceFromCP = abs((b->cp - coords).Magnitude());

				if (distanceFromP1 < 10) {
					selector = &b->p1;
				}

				else if (distanceFromP2 < 10) {
					selector = &b->p2;
				}
				else if (distanceFromCP < 10) {
					selector = &b->cp;
				}
			}
		}
		if (selector != nullptr &&
			GetKey(VK_LBUTTON).keyHeld) {
			selector->x = mouseInfo.x;
			selector->y = mouseInfo.y;
		}
		if (GetKey(VK_LBUTTON).keyReleased) {
			selector = nullptr;
		}
		if (GetKey(0x41).keyReleased) {
			Bezier b;
			b.p1 = NSMath2d::Vec2(10, GetScreenHeight() / 2);
			b.p2 = NSMath2d::Vec2(GetScreenWidth() - 50, GetScreenHeight() / 2);
			b.cp = NSMath2d::Vec2(GetScreenWidth() / 2, 20);
			bezierCurves.push_back(b);
		}
		if (GetKey(0x44).keyReleased) {
			bezierCurves.clear();
		}
		return true;
	}
};
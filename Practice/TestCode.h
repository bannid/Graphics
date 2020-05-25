#pragma once
#include "BEngine.h"
#include "Debug.h"
//Extend the BEngine class
class Test : public BEngine {
	void TestCodeForTextureLoading() {
		bool textureLoaded = LoadTexturePNG("C:\\Users\\Winny-Banni\\Pictures\\SpaceShip.png",
			space,true);
		if (textureLoaded) {
			OutputDebugString(L"Texture loaded");
		}
	}
	void TestDrawCircle() {
		color_t c = { 255,255,255 };
		FillCircle(GetScreenWidth() / 2, GetScreenHeight() / 2, 50, c);
	}
	void TestDrawRectangle() {
		
		int x = GetScreenWidth()/2;
		int y = GetScreenHeight()/2;
		color_t c = { 255,255,255 };
		DrawRectangle(x,y,x+100,y+100,c);
	}
	void TestCodeForLerp() {
		NSMath2d::Vec2 point1 = { 0,500 };
		NSMath2d::Vec2 point2 = { 500,500 };
		for (float t = 0; t < 1; t += 0.001) {
			auto temp = Lerp(point1, point2, t);
			color_t c = { 255,255,255 };
			SetPixel(temp.x, temp.y,c);
		}
		color_t r = { 255,0,0 };
		DrawLine(0, 500, 500, 500,r);
	}
	void TestCodeForBezierCurveRecursive() {
		color_t b = { 0,0,0 };
		color_t white = { 255,255,255 };
		ClearScreen(b);
		int screenHeight = GetScreenHeight();
		int screenWidth = GetScreenWidth();
		std::vector<NSMath2d::Vec2> temp = {
				{0,screenHeight / 2},
				{0,0},
				{screenWidth,0},
				{screenWidth,screenHeight / 2}
		};
		NSMath2d::Vec2 currentPoint = temp[0];
		for (float t = 0; t < 1; t += 0.01) {
			NSMath2d::Vec2 temp2(0, 0);
			BezierCurveRecursive(temp, t, temp2);
			DrawLine(currentPoint.x, currentPoint.y, temp2.x, temp2.y, white);
			currentPoint = temp2;
		}
	}
	void TestCodeForMousePos() {
		color_t black = { 0,0,0 };
		color_t white = { 255,255,255 };
		auto mouseCoords = GetMouseInfo();
		ClearScreen(black);
		DrawLine(0, 0, mouseCoords.x, mouseCoords.y - 31, white);
	}
	void TestCodeForGettingColorFromTexture() {
		int pixelDimensions = GetPixelDimension();
		int screenHeight = GetScreenHeight();
		int screenWidth = GetScreenWidth();
		for (int x = 0; x < screenWidth; x+= pixelDimensions) {
			for (int y = 0; y < screenHeight; y+=pixelDimensions) {
				float normalizedX =(float) x / screenWidth;
				float normalizedY = (float)y / screenHeight;
				color_t color = GetColorFromTexture(normalizedX, normalizedY, space);
				SetPixel(x, y, color);
		}
		}
	}
	void TestCodeForDrawingSprite() {
		color_t black = { 255,255,255 };
		ClearScreen(black);
		int velocity = 3;
		DrawSprite(*spaceShip, spaceShipPos);
		if (GetKey(VK_RIGHT).keyHeld) {
			spaceShipPos.x+= velocity;
		}
		if (GetKey(VK_LEFT).keyHeld) {
			spaceShipPos.x-= velocity;
		}
		if (GetKey(VK_DOWN).keyHeld) {
			spaceShipPos.y+= velocity;
		}
		if (GetKey(VK_UP).keyHeld) {
			spaceShipPos.y-= velocity;
		}
	}
	virtual bool OnUpdate(float elapsedTime) override {
		TestCodeForDrawingSprite();
		return true;
	}
	virtual bool OnCreate() override {
		TestCodeForTextureLoading(); 
		spaceShipPos = { GetScreenWidth() / 2,GetScreenHeight() / 2 };
		spaceShip = new Sprite(GetTexture(space),200,200,1);
		return true;
	}
private:
	TEXID space;
	Sprite * spaceShip;
	NSMath2d::Vec2 spaceShipPos;
};

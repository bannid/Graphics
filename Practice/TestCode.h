#pragma once
#include "BEngine.h"
#include "Debug.h"
//Extend the BEngine class
class Test : public BEngine {
	void TestCodeForKeys() {
		NSInput::Key keyA = GetKey(VK_LBUTTON);
		if (keyA.keyReleased) {
			if (GetClearColorRGBPacked() == NSColors::YELLOW) {
				SetClearColor(NSColors::WHITE);
			}
			else {
				SetClearColor(NSColors::YELLOW);
			}
		}
		ClearScreen();
	}
	void TestCodeForTextureLoading() {
		bool textureLoaded = LoadTexturePNG("C:\\Users\\Winny-Banni\\Pictures\\SpaceShip.png",
			space,true);
		if (textureLoaded) {
			OutputDebugString(L"Texture loaded");
		}
	}
	void TestDrawCircle() {
		SetClearColor(NSColors::BLACK);
		SetColor(NSColors::YELLOW);
		//DrawCircle(GetScreenWidth()/2, GetScreenHeight()/2, 70);
		FillCircle(GetScreenWidth() / 2, GetScreenHeight() / 2, 50);
	}
	void TestDrawRectangle() {
		SetClearColor(NSColors::BLACK);
		SetColor(NSColors::YELLOW);
		int x = GetScreenWidth()/2;
		int y = GetScreenHeight()/2;
		DrawRectangle(x,y,x+100,y+100);
	}
	void TestCodeForLerp() {
		NSMath2d::Vec2 point1 = { 0,500 };
		NSMath2d::Vec2 point2 = { 500,500 };
		for (float t = 0; t < 1; t += 0.001) {
			auto temp = Lerp(point1, point2, t);
			SetColor(NSColors::YELLOW);
			SetPixel(temp.x, temp.y);
		}
		SetColor(NSColors::RED);
		DrawLine(0, 500, 500, 500);
	}
	void TestCodeForBezierCurveRecursive() {
		SetClearColor(NSColors::BLACK);
		ClearScreen();
		SetColor(NSColors::WHITE);
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
			DrawLine(currentPoint.x, currentPoint.y, temp2.x, temp2.y);
			currentPoint = temp2;
		}
	}
	void TestCodeForMousePos() {
		SetColor(NSColors::WHITE);
		SetClearColor(NSColors::BLACK);
		auto mouseCoords = GetMouseInfo();
		ClearScreen();
		DrawLine(0, 0, mouseCoords.x, mouseCoords.y - 31);
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
		ClearScreen();
		int velocity = 3;
		DrawSprite(*spaceShip);
		if (GetKey(VK_RIGHT).keyHeld) {
			spaceShip->pos.x+= velocity;
		}
		if (GetKey(VK_LEFT).keyHeld) {
			spaceShip->pos.x-= velocity;
		}
		if (GetKey(VK_DOWN).keyHeld) {
			spaceShip->pos.y+= velocity;
		}
		if (GetKey(VK_UP).keyHeld) {
			spaceShip->pos.y-= velocity;
		}
	}
	virtual bool OnUpdate(float elapsedTime) override {
		TestCodeForDrawingSprite();
		return true;
	}
	virtual bool OnCreate() override {
		TestCodeForTextureLoading(); 
		spaceShip = new Sprite(GetTexture(space), { GetScreenWidth()/2,GetScreenHeight()/2},100,100,1);
		return true;
	}
private:
	TEXID space;
	Sprite * spaceShip;
};

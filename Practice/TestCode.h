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
		BColors::color_t c = { 255,255,255 };
		FillCircle(GetScreenWidth() / 2, GetScreenHeight() / 2, 50, c);
	}
	void TestDrawRectangle() {
		
		int x = GetScreenWidth()/2;
		int y = GetScreenHeight()/2;
		BColors::color_t c = { 255,255,255 };
		DrawRectangle(x,y,x+100,y+100,c);
	}
	void TestDrawTriangle() {
		//FillTriangle(500,0,300,100,500,500);
		//FillTriangle(500, 500, 300, 100, 500, 0);
		for (auto it = triangles.begin(); it != triangles.end(); it++) {
			FillTriangle(*it);
		}
	}
	bool TestCodeForLoadObjFile() {
		return LoadOBJFile("C:\\Users\\Winny-Banni\\Desktop\\videos\\teapot.obj");
	}
	void TestCodeForLerp() {
		BMath::Vec2 point1 = { 0,500 };
		BMath::Vec2 point2 = { 500,500 };
		for (float t = 0; t < 1; t += 0.001) {
			auto temp = Lerp(point1, point2, t);
			BColors::color_t c = { 255,255,255 };
			SetPixel(temp.x, temp.y,c);
		}
		BColors::color_t r = { 255,0,0 };
		DrawLine(0, 500, 500, 500,r);
	}
	void TestCodeForBezierCurveRecursive() {
		BColors::color_t b = { 0,0,0 };
		BColors::color_t white = { 255,255,255 };
		ClearScreen(b);
		int screenHeight = GetScreenHeight();
		int screenWidth = GetScreenWidth();
		std::vector<BMath::Vec2> temp = {
				{0,screenHeight / 2},
				{0,0},
				{screenWidth,0},
				{screenWidth,screenHeight / 2}
		};
		BMath::Vec2 currentPoint = temp[0];
		for (float t = 0; t < 1; t += 0.01) {
			BMath::Vec2 temp2(0, 0);
			BezierCurveRecursive(temp, t, temp2);
			DrawLine(currentPoint.x, currentPoint.y, temp2.x, temp2.y, white);
			currentPoint = temp2;
		}
	}
	void TestCodeForMousePos() {
		BColors::color_t black = { 0,0,0 };
		BColors::color_t white = { 255,255,255 };
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
				BColors::color_t color = GetColorFromTexture(normalizedX, normalizedY, space);
				SetPixel(x, y, color);
		}
		}
	}
	void TestCodeForDrawingSprite() {
		BColors::color_t black = { 255,255,255 };
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
		//TestCodeForDrawingSprite();
		TestDrawTriangle();
		return true;
	}
	virtual bool OnCreate() override {
		//TestCodeForTextureLoading(); 
		//spaceShipPos = { GetScreenWidth() / 2,GetScreenHeight() / 2 };
		//spaceShip = new Sprite(GetTexture(space),200,200,1);
		return TestCodeForLoadObjFile();
	}
private:
	TEXID space;
	Sprite * spaceShip;
	BMath::Vec2 spaceShipPos;
};

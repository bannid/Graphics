#pragma once
#include "BEngine.h"
#include "Debug.h"
#include "Utils.h"
//Extend the BEngine class
class Test : public BEngine {
	void TestCodeForTextureLoading() {
		bool textureLoaded = BUtils::LoadTexturePNG("C:\\Users\\Winny-Banni\\Pictures\\Fonts.png",
			space);
		if (textureLoaded) {
			OutputDebugString(L"Texture loaded");
		}
	}
	void TestDrawCircle() {
		BColor c = { 1.0f,1.0f,1.0f };
		FillCircle(GetScreenWidth() / 2, GetScreenHeight() / 2, 50, c);
	}
	void TestDrawRectangle() {
		
		int x = GetScreenWidth()/2;
		int y = GetScreenHeight()/2;
		BColor c = { 1.0f,1.0f,1.0f };
		DrawRectangle(x,y,x+100,y+100,c);
	}
	void TestDrawTriangle() {
		//FillTriangle(500,0,300,100,500,500);
		//FillTriangle(500, 500, 300, 100, 500, 0);
		/*for (auto it = triangles.begin(); it != triangles.end(); it++) {
			FillTriangle(*it);
		}*/
	}
	bool TestCodeForLoadObjFile() {
		return true;// return LoadOBJFile("C:\\Users\\Winny-Banni\\Desktop\\videos\\teapot.obj");
	}
	
	void TestCodeForBezierCurveRecursive() {
		BColor bBColor = { 0.0f,0.0f,0.0f };
		BColor white = { 1.0f,1.0f,1.0f };
		ClearScreen(bBColor);
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
		BColor black = { 0,0,0 };
		BColor white = { 1.0f,1.0f,1.0f };
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
				BColor color = BUtils::GetColorFromTexture(normalizedX, normalizedY, space);
				SetPixel(x, y, color);
		}
		}
	}
	void TestCodeForDrawingSprite() {
		BColor black = { 1.0f,1.0f,1.0f };
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
	Texture * space;
	Sprite * spaceShip;
	BMath::Vec2 spaceShipPos;
};

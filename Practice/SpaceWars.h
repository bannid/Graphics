#pragma once
#include "BEngine.h"

class SpaceWars : public BEngine {
	const char * spaceShipTextureFile = "C:\\Users\\Winny-Banni\\Pictures\\SpaceShip.png";
	const char * bulletTextureFile = "C:\\Users\\Winny-Banni\\Pictures\\bullet.png";
	
	TEXID spaceshipId;
	TEXID bulletId;
	
	Sprite * spaceShip;
	Sprite * bullet;
	
	NSMath2d::Vec2 spaceShipPos;
	NSMath2d::Vec2 velocity;
	NSMath2d::Vec2 bulletVelocity = { 0,-5 };
	std::vector<NSMath2d::Vec2> bullets;
	
	int numberOfBullets = 0;
	int maxNumberOfBullets = 30;
	int velocityIncrement = 10;
	float dampening = 0.99;
	bool LoadTextureFiles() {
		if (LoadTexturePNG("C:\\Users\\Winny-Banni\\Pictures\\SpaceShip.png",
			spaceshipId, true) &&
			LoadTexturePNG("C:\\Users\\Winny-Banni\\Pictures\\bullet.png",
				bulletId, true)) {
			spaceShip = new Sprite(GetTexture(spaceshipId), 150, 150, 1);
			bullet = new Sprite(GetTexture(bulletId), 40, 20, 1);
		}
		else {
			return false;
		}
		return true;
	}
	void DrawBullets() {
		for (int i = 0; i < numberOfBullets; i++) {
			DrawSprite(*bullet, bullets[i]);
		}
	}
	void UpdateBullets() {
		for (int i = 0; i < numberOfBullets; i++) {
			bullets[i].Add(bulletVelocity);
		}
		for (int i = 0; i < numberOfBullets; i++) {
			if (bullets[i].y < 0) {
				std::swap(bullets[i], bullets[numberOfBullets - 1]);
				numberOfBullets--;
			}
		} 
	}
	virtual bool OnCreate() override {
		bool assetsLoaded = LoadTextureFiles();
		spaceShipPos.x = GetScreenWidth() / 2;
		spaceShipPos.y = GetScreenHeight() - spaceShip->height / 2 - 100;
		bullets = std::vector<NSMath2d::Vec2>(maxNumberOfBullets);
		return assetsLoaded;
	}

	virtual bool OnUpdate(float elapsedTime) override {
		ClearScreen(NSColors::BLACK);
		DrawSprite(*spaceShip,spaceShipPos);
		auto velocityScaled = velocity * elapsedTime;
		spaceShipPos.Add(velocityScaled);
		if (GetKey(VK_RIGHT).keyHeld) {
			velocity.x+= velocityIncrement;
		}
		if (GetKey(VK_LEFT).keyHeld) {
			velocity.x-= velocityIncrement;
		}
		if (GetKey(VK_SPACE).keyReleased &&
			numberOfBullets < maxNumberOfBullets) {
			bullets[numberOfBullets] = { spaceShipPos.x,spaceShipPos.y };
			numberOfBullets++;
		}
		velocity = velocity * dampening;
		DrawBullets();
		UpdateBullets();
		return true;
	}
};

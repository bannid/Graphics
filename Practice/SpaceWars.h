#pragma once
#include "BEngine.h"
#include <stdlib.h>
#define DEBUG_DRAW 0
class SpaceWars : public BEngine {
	//################ Assets ####################
	const char * spaceShipTextureFile = "C:\\Users\\Winny-Banni\\Pictures\\SpaceShip.png";
	const char * bulletTextureFile = "C:\\Users\\Winny-Banni\\Pictures\\bullet.png";
	const char * enemyShipTextureFile = "C:\\Users\\Winny-Banni\\Pictures\\enemyShip.png";
	
	TEXID spaceshipId;
	TEXID bulletId;
	TEXID enemyShipId;

	Sprite * spaceShip;
	Sprite * bullet;
	Sprite * enemyShip;
	//################ End Assets##################
	//################ Player    ##################
	BMath::Vec2 spaceShipPos;
	BMath::Vec2 shipVelocity;
	int velocityIncrement = 10;
	int spaceShipHeight;
	int spaceShipWidth;
	float shipVelocityDampening = 0.99;

	BMath::Vec2 bulletVelocity = { 0,-1000 };
	std::vector<BMath::Vec2> bullets;
	int numberOfBullets = 0;
	int maxNumberOfBullets = 30;
	int bulletHeight;
	int bulletWidth;
	
	//############### End player ###################
	//############### Enemy ships ##################
	std::vector<BMath::Vec2> enemyShips;
	BMath::Vec2 shipsVelocity = { 0,100 };
	int maximumNumberOfEnemyShips = 10;
	int numberOfShips = 0;
	int enemyShipRadius = 50;
	//############### End enemy ships ##############
	bool LoadTextureFiles() {
		if (LoadTexturePNG(spaceShipTextureFile,
			spaceshipId, true) &&
			LoadTexturePNG(bulletTextureFile,
				bulletId, true) &&
			LoadTexturePNG(enemyShipTextureFile,
				enemyShipId, true)) {
			spaceShip = new Sprite(GetTexture(spaceshipId), spaceShipHeight, spaceShipWidth, 1);
			bullet = new Sprite(GetTexture(bulletId), bulletHeight, bulletWidth, 1);
			enemyShip = new Sprite(GetTexture(enemyShipId), enemyShipRadius, enemyShipRadius, 1);
			enemyShip->SetTinting({ 255,255,0 }, 0.3);

		}
		else {
			return false;
		}
		return true;
	}
	inline void DrawBullets() {
		for (int i = 0; i < numberOfBullets; i++) {
			DrawSprite(*bullet, bullets[i]);
		}
	}
	inline void UpdateBullets(float elapasedTime) {
		for (int i = 0; i < numberOfBullets; i++) {
			auto bulletVelocityScaled = bulletVelocity * elapasedTime;
			bullets[i].Add(bulletVelocityScaled);
		}
		for (int i = 0; i < numberOfBullets; i++) {
			if (bullets[i].y < 0) {
				std::swap(bullets[i], bullets[numberOfBullets - 1]);
				numberOfBullets--;
			}
		} 
	}
	inline void DrawPlayerShip() {
#if DEBUG_DRAW
		DrawCircle(spaceShipPos, spaceShipWidth * 0.5, NSColors::WHITE);
#endif
		DrawSprite(*spaceShip, spaceShipPos);
	}
	inline void UpdatePlayerShip(float elapsedTime) {
		auto velocityScaled = shipVelocity * elapsedTime;
		spaceShipPos.Add(velocityScaled);
		if (GetKey(VK_RIGHT).keyHeld) {
			shipVelocity.x += velocityIncrement;
		}
		if (GetKey(VK_LEFT).keyHeld) {
			shipVelocity.x -= velocityIncrement;
		}
		if (GetKey(VK_SPACE).keyReleased &&
			numberOfBullets < maxNumberOfBullets) {
			bullets[numberOfBullets] = { spaceShipPos.x,spaceShipPos.y };
			numberOfBullets++;
		}
		shipVelocity = shipVelocity * shipVelocityDampening;
	}
	inline void AssignRandomPos(BMath::Vec2 & pos) {
		int screenWidth = GetScreenWidth();
		float randomX = (float)rand() / RAND_MAX;
		float randomY = (float)rand() / RAND_MAX;
		int randomX2 = 50 + (randomX * (screenWidth - 50));
		int randomY2 = -(50 + (randomX * (screenWidth - 50)));
		pos = { randomX2 ,randomY2 };
	}
	inline void PopulateEnemyShips() {
		if (numberOfShips < maximumNumberOfEnemyShips) {
			AssignRandomPos(enemyShips[numberOfShips]);
			numberOfShips++;
		}
	}
	inline void DrawShips(float elapsedTime) {
		for(int i = 0; i< numberOfShips; i++){
			DrawSprite(*enemyShip, enemyShips[i]);
#if DEBUG_DRAW
			DrawCircle(enemyShips[i], enemyShipRadius * 0.6, NSColors::WHITE);
#endif
			auto velocityScaled = shipsVelocity * elapsedTime;
			enemyShips[i].Add(velocityScaled);
		}
	}
	inline void UpdateEnemyShips(float elapsedTime) {
		for (int i = 0; i < numberOfShips; i++) {
			if (enemyShips[i].y > GetScreenHeight()) {
				AssignRandomPos(enemyShips[i]);
			}
			auto velocityScaled = shipsVelocity * elapsedTime;
			enemyShips[i].Add(velocityScaled);
		}
	}
	inline void CollisionDetection() {
		for (int i = 0; i < numberOfShips; i++) {
			for (int j = 0; j < numberOfBullets; j++) {
				if ((enemyShips[i] - bullets[j]).Magnitude() < enemyShipRadius * 0.6) {
					AssignRandomPos(enemyShips[i]);
				}
			}
		}
	}
	virtual bool OnCreate() override {
		int screenHeight = GetScreenHeight();
		int screenWidth = GetScreenWidth();
		this->spaceShipHeight = (screenWidth * 0.15) * ((float)screenHeight / screenWidth);
		this->spaceShipWidth = (screenWidth * 0.15) * ((float)screenHeight / screenWidth);
		this->bulletHeight = this->spaceShipHeight * 0.2;
		this->bulletWidth = this->spaceShipWidth * 0.1;
		bool assetsLoaded = LoadTextureFiles();
		spaceShipPos.x = GetScreenWidth() / 2;
		spaceShipPos.y = GetScreenHeight() - (spaceShipHeight/2);
		bullets = std::vector<BMath::Vec2>(maxNumberOfBullets);
		enemyShips = std::vector<BMath::Vec2>(maximumNumberOfEnemyShips);
		return assetsLoaded;
	}

	virtual bool OnUpdate(float elapsedTime) override {
		ClearScreen(BColors::BLACK);
		DrawPlayerShip();
		UpdatePlayerShip(elapsedTime);
		DrawBullets();
		UpdateBullets(elapsedTime);
		PopulateEnemyShips();
		DrawShips(elapsedTime);
		UpdateEnemyShips(elapsedTime);
		CollisionDetection();
		return true;
	}
	virtual bool OnDestroy() override {
		//No need to free the memory.  OS will take care of it.
		/*delete spaceShip;
		delete bullet;
		delete enemyShip;*/
		return true;
	}
};

#undef DEBUG_DRAW
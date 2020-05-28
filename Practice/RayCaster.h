#pragma once
#include <string>
#include "BEngine.h"

#define DEBUG_DRAW 1

class RayCaster : public BEngine {
	//Dimensions of our world
	int mapWidth = 16;
	int mapHeight = 16;
	int windowWidth = 528;
	int windowHeight = 528;
	//Calculated in the OnCreate function
	int rectangleHeight;
	int rectangleWidth;
	//Player postions
	float playerX = 5;
    float playerY = 5;
	//Player size
	int playerSize = 5;
	//Viewing angle of player
	float playerAngle = 0.0f;
	//Field of view in radians
	float fov = 1.0f;
	std::vector<int> colors = { BColors::MAROON,BColors::RED,BColors::GREEN,BColors::YELLOW };
	//Map of our world -> 16x16
	std::string map =   "0000000000000000"\
						"0   3          0"\
						"0   3          0"\
						"0      3       0"\
						"0      3       0"\
						"0      3       0"\
						"0              0"\
						"0              0"\
						"0              0"\
						"0              0"\
						"0   2          0"\
						"0   2          0"\
						"0   2          0"\
						"0   222222222220"\
						"0              0"\
						"0000222222222220";
	void DrawMap() {
		for (int i = 0; i < mapHeight; i++) {
			for (int j = 0; j < mapWidth; j++) {
				
				if (map[j + i * mapWidth] == ' ') continue;
#if DEBUG_DRAW
				int colorIndex = map[j + i * mapWidth] - '0';
				FillRectangle(j * rectangleWidth,
					i * rectangleHeight,
					j * rectangleWidth + rectangleWidth,
					i * rectangleHeight + rectangleHeight, colors[colorIndex] );
#endif
			}
		}
	}
	void DrawPlayer() {
		int remappedX = playerX * rectangleWidth;
		int remappedY = playerY * rectangleHeight;
#if DEBUG_DRAW
		FillRectangle(remappedX, remappedY, remappedX + playerSize, remappedY + playerSize, BColors::WHITE);
#endif
		float a = playerAngle - fov / 2; 
		for (int i = 0; i < windowWidth; i++) {
			float gazeDirectionX = std::cos(a);
			float gazeDirectionY = std::sin(a);
			float c = 0;
			int colorIndex = 0;
			for (; c < 20; c += 0.01f) {
				int x = playerX + (gazeDirectionX * c);
				int y = playerY + (gazeDirectionY * c);
				if (map[x + y * mapWidth] != ' ') {
					colorIndex = map[x + y * mapWidth] - '0';
					break;
				}
			}
#if DEBUG_DRAW
			DrawLine(remappedX, remappedY, remappedX + (c * gazeDirectionX* rectangleWidth), remappedY + (c *gazeDirectionY* rectangleHeight), BColors::WHITE);
#endif
			int screenOffsetX = 600;
			int segmentHeight = 300 / ((c * std::cos(a-playerAngle)));
			DrawLine(screenOffsetX + i, GetScreenHeight() / 1.5, screenOffsetX + i, GetScreenHeight() / 1.5 - segmentHeight, colors[colorIndex]);
			a += fov / windowWidth;
			
		}
		
	}
	void HandleInput(float elapsedTime) {
		GetKey(VK_RIGHT).keyDown ? playerAngle += 1 * elapsedTime : playerAngle = playerAngle;
		GetKey(VK_LEFT).keyDown ? playerAngle -= 1 * elapsedTime : playerAngle = playerAngle;
		float playerSpeed = 2.0f;
		if (GetKey(VK_UP).keyDown) {
			float directionX = std::cos(playerAngle);
			float directionY = std::sin(playerAngle);
			float newX = playerX + directionX * elapsedTime * playerSpeed;
			float newY = playerY + directionY * elapsedTime * playerSpeed;
			if (map[int(newX) + int(newY) * mapWidth] == ' ') {
				playerX = newX;
				playerY = newY;
			}
		}
		if (GetKey(VK_DOWN).keyDown) {
			float directionX = std::cos(playerAngle);
			float directionY = std::sin(playerAngle);
			float newX = playerX - directionX * elapsedTime * playerSpeed;
			float newY = playerY - directionY * elapsedTime * playerSpeed;
			if (map[int(newX) + int(newY) * mapWidth] == ' ') {
				playerX = newX;
				playerY = newY;
			}
		}
		if (GetKey('A').keyDown) {
			fov += 0.2f * elapsedTime;
		}
		if (GetKey('S').keyDown) {
			fov -= 0.2f * elapsedTime;
		}
	}
	virtual bool OnCreate() override {
		rectangleHeight = windowHeight / mapHeight;
		rectangleWidth = windowWidth / mapWidth;
		return true;
	}

	virtual bool OnUpdate(float elapsedTime) override {
		ClearScreen(BColors::BLACK);
		DrawMap();
		DrawPlayer();
		HandleInput(elapsedTime);
		return true;
	}
};
#undef DEBUG_DRAW
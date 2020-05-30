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
	std::vector<int> colors = {
		BColors::MAROON,
		BColors::RED,
		BColors::GREEN,
		BColors::YELLOW,
		BColors::RED,
		BColors::BLUE
	};
	Texture walls;
	//Map of our world -> 16x16
	std::string map =   "1111111111111111"\
						"1   4          1"\
						"1   4          1"\
						"1      4       1"\
						"1      4       1"\
						"1      4       1"\
						"1              1"\
						"1              1"\
						"1              1"\
						"1              1"\
						"1   444        1"\
						"1   4 4444444  1"\
						"1   4          1"\
						"1   444444444441"\
						"1              1"\
						"1111111111111111";
	void DrawMap() {
		for (int i = 0; i < mapHeight; i++) {
			for (int j = 0; j < mapWidth; j++) {

				if (map[j + i * mapWidth] == ' ') continue;
#if DEBUG_DRAW
				int colorIndex = map[j + i * mapWidth] - '0';
				FillRectangle(j * rectangleWidth,
					i * rectangleHeight,
					j * rectangleWidth + rectangleWidth,
					i * rectangleHeight + rectangleHeight, colors[colorIndex]);
#endif
			}
		}
	}
	void DrawPlayer() {
		int remappedX = playerX * rectangleWidth;//Remapped X and Y to our world which is 16x16
		int remappedY = playerY * rectangleHeight;
#if DEBUG_DRAW
		FillRectangle(remappedX, remappedY, remappedX + playerSize, remappedY + playerSize, BColors::WHITE);
#endif
		//This section draws the vision cone
		//First we set the first ray angle to 
		//current player angle - field of view / 2
		float a = playerAngle - fov / 2;
		for (int i = 0; i < windowWidth; i++) {
			//Now we get the rayDirection
			float gazeDirectionX = std::cos(a);
			float gazeDirectionY = std::sin(a);
			float c = 0;
			int colorIndex = 0;
			float hitX;
			float hitY;
			float textureCoord;
			//We increase the c until we hit something
			for (; c < 20; c += 0.01f) {
				float cx = playerX + (gazeDirectionX * c);
				float cy = playerY + (gazeDirectionY * c);
				int x = cx;
				int y = cy;
				if (map[x + y * mapWidth] != ' ') {
					//Here we want to know which point is 
					//closest to the near integer value.
					hitX = cx - floor(cx + .5);
					hitY = cy - floor(cy + .5);
					textureCoord = hitX;
					if (std::abs(hitY) > std::abs(hitX)) {
						textureCoord = hitY;
					}
					textureCoord = std::abs(textureCoord);
					colorIndex = map[x + y * mapWidth] - '0';
					break;
				}
			}
#if DEBUG_DRAW
			DrawLine(remappedX, remappedY, remappedX + (c * gazeDirectionX* rectangleWidth), remappedY + (c *gazeDirectionY* rectangleHeight), BColors::WHITE);
			if (std::abs(a - playerAngle) < 0.01) {
				DrawLine(remappedX, remappedY, remappedX + (c * gazeDirectionX* rectangleWidth), remappedY + (c *gazeDirectionY* rectangleHeight), BColors::RED);
			}
#endif

			int screenOffsetX = 600;
			//We multiply by c with cosine of angle b/w ray and player viewing
			//angle because we want players distance not the ray distance
			int segmentHeight = 300 / ((c * std::cos(a - playerAngle)));
			float percentageOfOneTexture = (float) 64.0f / walls.width;
			float remappedTextureCoord = (textureCoord / 6.0f) + percentageOfOneTexture * colorIndex;
			int startingY = GetScreenHeight() - 100;
			for (int y = startingY; y > startingY - segmentHeight; y--) {
				float yTextureCoord = (float)std::abs(y - startingY) / segmentHeight;
				BColors::color_t color = GetColorFromTexture(remappedTextureCoord, yTextureCoord, &walls);
				SetPixel(screenOffsetX + i, y, color);
			}
			//Increase the ray angle with respect to the screen width.
			//Screen width decides how many rays we are shooting.  We
			//will always cover fov because this loop will run windowWidth times.
			//I.e. in this cases 528 times. and fov / 528 * 528 = fov
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
		return LoadTexturePNG("C:\\Users\\Winny-Banni\\Pictures\\walltext.png", &walls, true);
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
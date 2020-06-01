#pragma once
#include <string>
#include "BEngine.h"

#define DEBUG_DRAW 1
#define TAU 6.28318530718
#define M_PI 3.1415926535
struct Monster {
	float x;
	float y;
	Texture * texture;
	unsigned int indexInTexture;
	Monster(float x, float y, Texture * tex, unsigned int index) :x(x), y(y), texture(tex), indexInTexture(index) {}
};
class RayCaster : public BEngine {
	//Dimensions of our world
	int mapWidth = 16;
	int mapHeight = 16;
	int windowWidth = 0;
	int windowHeight = 0;

	int screenOffsetX = 0;
	//Monsters
	std::vector<Monster> monsters;
	bool monsterVisible = false;
	std::string toDraw;
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
	float fov = 1.11f;
	std::vector<int> colors = {
		BColors::MAROON,
		BColors::RED,
		BColors::GREEN,
		BColors::YELLOW,
		BColors::RED,
		BColors::BLUE
	};
	Texture walls;
	Texture monstersTex;
	//Map of our world -> 16x16
	std::string map = "1111111131111111"\
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
	float * depth;
	void DrawMap() {
		for (int i = 0; i < mapHeight; i++) {
			for (int j = 0; j < mapWidth; j++) {

				if (map[j + i * mapWidth] == ' ') continue;

				int colorIndex = map[j + i * mapWidth] - '0';
				FillRectangle(j * rectangleWidth,
					i * rectangleHeight,
					j * rectangleWidth + rectangleWidth,
					i * rectangleHeight + rectangleHeight, colors[colorIndex]);

			}
		}
	}
	void DrawPlayer() {

		//This section draws the vision cone
		//First we set the first ray angle to 
		//current player angle - field of view / 2
		float a = playerAngle - fov / 2;
		float previousSegmentHeight = -1;
		for (int i = 0; i < windowWidth; i++) {
			//Now we get the rayDirection
			float gazeDirectionX = std::cos(a);
			float gazeDirectionY = std::sin(a);
			float c = 0;
			int colorIndex = 0;
			float hitX;
			float hitY;
			float textureCoord;
			//We increase the c until we hit something or 
			// the maxium value is reached.
			for (; c < 20; c += 0.01f) {
				float cx = playerX + (gazeDirectionX * c);
				float cy = playerY + (gazeDirectionY * c);
				int x = cx;
				int y = cy;
				if (map[x + y * mapWidth] != ' ') {
					depth[i] = c;
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
			//We multiply by c with cosine of angle b/w ray and player viewing
			//angle because we want players distance not the ray distance.
			//We want x in this case not y, otherwise we would have fish eye effect.
			//As the ray angle approaches player angle, cosine of c*player_angle-angle aprroaches 1.
			/*
					---------------------
					  \    |
					   \   |
					   y\  |x
						 \ |
						  \|
			*/
			float cosined = c * (std::cosf(a - playerAngle));
			int segmentHeight = windowHeight / cosined;
			//We have 6 textures inside one with 64x64 dimensions
			float percentageOfOneTexture = (float) 64.0f / walls.width;
			//We remap the texture coords by dividing it by 6 and then adding
			//the current index in hit postion multiplied by the area covered by one texture.
			float remappedTextureCoord = (textureCoord / 6.0f) + percentageOfOneTexture * colorIndex;
			//Starting pos to draw lines on y axis
			int startingY = windowHeight / 2 + segmentHeight / 2;
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

#if DEBUG_DRAW
			int remappedX = playerX * rectangleWidth;//Remapped X and Y to our world which is 16x16
			int remappedY = playerY * rectangleHeight;
			FillRectangle(remappedX, remappedY, remappedX + playerSize, remappedY + playerSize, BColors::WHITE);

			DrawLine(remappedX, remappedY, remappedX + (c * gazeDirectionX* rectangleWidth), remappedY + (c *gazeDirectionY* rectangleHeight), BColors::WHITE);
			if (std::abs(a - playerAngle) < 0.01) {
				DrawLine(remappedX, remappedY, remappedX + (c * gazeDirectionX* rectangleWidth), remappedY + (c *gazeDirectionY* rectangleHeight), BColors::RED);
			}
#endif
		}
		for (auto it = monsters.begin(); it != monsters.end(); it++) {
			float vectorXOfMonster = it->x - playerX;
			float vectorYOfMonster = it->y - playerY;
			float monsterDistance = std::sqrt(vectorXOfMonster * vectorXOfMonster + vectorYOfMonster * vectorYOfMonster);
			if (monsterDistance < 1.0f)continue;
			float spriteAngle = std::atan2(vectorYOfMonster, vectorXOfMonster);
			float spriteAngleBwPlayerGazeDirecton = spriteAngle - playerAngle;
			if (spriteAngleBwPlayerGazeDirecton > M_PI) spriteAngleBwPlayerGazeDirecton -= 2 * M_PI;
			if (spriteAngleBwPlayerGazeDirecton < -M_PI) spriteAngleBwPlayerGazeDirecton += 2 * M_PI;
			int spriteSize = (windowHeight) / monsterDistance;
			if (spriteSize > 1000) {
				spriteSize = 1000;
			}
			//HOffset is calculated by determining how much percentagle of fov/2 is taken
			//by the angle bw monster and the player. and then we multiply it by windowWidth/2
			//because half of fov (fov/2) takes half of the screen.
			int hOffset = (spriteAngleBwPlayerGazeDirecton / (fov / 2))*(windowWidth / 2);
			int vOffset = spriteSize / 2;
			int startingX = windowWidth / 2 + hOffset - spriteSize / 2;
			int endingX = windowWidth / 2 + hOffset + spriteSize;
			int startingY = windowHeight / 2 - vOffset;
			int endingY = windowHeight / 2 + spriteSize;
			for (int x = startingX; x < endingX; x++) {
				if (x < 0 || x > windowWidth)continue;
				for (int y = startingY; y < endingY; y++) {
					if (y < 0 || y > windowHeight)continue;
					float normalizedX = (float)(x - startingX) / (endingX - startingX);
					float normalizedY = (float)(y - startingY) / (endingY - startingY);
					float percentageOfOneTexture = (float)64 / monstersTex.width;
					normalizedX = normalizedX / 4.0f + percentageOfOneTexture * it->indexInTexture;
					BColors::color_t color = GetColorFromTexture(normalizedX, normalizedY, &monstersTex);
					SetBlendingMode(ALPHA);
					if (depth[x] < monsterDistance)continue;
					SetPixel(x, y, color);
					SetBlendingMode(NORMAL);
				}
			}
			int remappedXForMonster = it->x * rectangleWidth;
			int remppaedYForMonster = it->y * rectangleHeight;
			FillRectangle(remappedXForMonster, remppaedYForMonster, remappedXForMonster + playerSize, remppaedYForMonster + playerSize, BColors::RED);
		}

	}
	void HandleInput(float elapsedTime) {
		float playerSpeed = 2.0f;
		if (GetKey(VK_UP).keyDown || GetKey(VK_LBUTTON).keyDown) {
			float directionX = std::cos(playerAngle);
			float directionY = std::sin(playerAngle);
			float newX = playerX + directionX * elapsedTime * playerSpeed;
			float newY = playerY + directionY * elapsedTime * playerSpeed;
			if (map[int(newX) + int(newY) * mapWidth] == ' ') {
				playerX = newX;
				playerY = newY;
			}
		}
		if (GetKey(VK_DOWN).keyDown || GetKey(VK_RBUTTON).keyDown) {
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
		int leftOffset = windowWidth / 2 - 10;
		int rightOffset = windowWidth / 2 + 10;
		if (mouseInfo.x < leftOffset) {
			playerAngle -= 1 * elapsedTime * ((float)(leftOffset - mouseInfo.x) / leftOffset);
		}
		else if (mouseInfo.x > rightOffset) {
			playerAngle += 1 * elapsedTime * ((float)(mouseInfo.x - leftOffset) / rightOffset);
		}
		if (playerAngle > TAU || playerAngle < -TAU) {
			playerAngle = 0.0f;
		}
	}
	void PopulateNonPlayerCharacters() {
		monsters.push_back(Monster(10.0f, 10.0f, &monstersTex, 0));
		monsters.push_back(Monster(8.0f, 8.0f, &monstersTex, 2));
		monsters.push_back(Monster(6.0f, 6.0f, &monstersTex, 1));
		monsters.push_back(Monster(4.0f, 5.0f, &monstersTex, 3));

	}
	virtual bool OnCreate() override {
		windowHeight = GetScreenHeight();
		windowWidth = GetScreenWidth();
		depth = new float[windowWidth];
		rectangleHeight = 10;
		rectangleWidth = 10;
		PopulateNonPlayerCharacters();
		return LoadTexturePNG("C:\\Users\\Winny-Banni\\Pictures\\walltext.png", &walls, true) &&
			LoadTexturePNG("C:\\Users\\Winny-Banni\\Pictures\\monsters.png", &monstersTex, true);
	}

	virtual bool OnUpdate(float elapsedTime) override {
		ClearScreen(BColors::GRAY);

		DrawPlayer();
#if DEBUG_DRAW
		DrawMap();
#endif
		HandleInput(elapsedTime);
		DrawString(toDraw, 0, 0, 50, BColors::WHITE);
		return true;
	}
	virtual bool OnDestroy() {
		delete depth;
		return true;
	}
};
#undef DEBUG_DRAW
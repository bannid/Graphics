#pragma once
#include "BEngine.h"
class Perlin;
#define MFPTR(a,params) (this->*a)(params)
typedef float(Perlin::*LerpMethod)(float);
class Perlin : public BEngine {
	bool useCosine = false;
	int outX;
	int outY;
	int screenHeight = 800;
	int screenWidth = 800;
	int cellSizeL = 20;
	std::vector<BMath::Vec2> gradientVecs;
	LerpMethod m = &Perlin::CosineInterpolation;
	float randomValues[800];
	float CosineInterpolation(float t) {
		return 1 - (((std::cos(t * 3.15) + 1)) / 2);
	}
	float UnderrootFunction(float t) {
		return std::sqrt(t);
	}
	void DrawColorsUsingInterpolation() {
		int leftTopX = 100;
		int leftTopY = 100;
		int height = 400;
		int width = 400;
		BColors::color_t colorOnLeftEdge = { 255,0,0 };
		BColors::color_t colorOnRightEdge = { 0,255,0 };
		BColors::color_t colorOnLowerEdge = { 0,0,255 };
		BColors::color_t colorOnLowerRightEdge = { 255,255,0 };
		for (int x = leftTopX; x < leftTopX + width; x++) {
			for (int y = leftTopY; y < leftTopY + height; y++) {
				float alphaX = 1 - (float)(x - leftTopX) / width;
				if (useCosine) {
					alphaX = CosineInterpolation(alphaX);
				}
				float betaX = 1 - alphaX;
				BColors::color_t upperColor;
				BColors::color_t lowerColor;
				BColors::color_t finalColor;
				upperColor.red = alphaX * colorOnLeftEdge.red + betaX * colorOnRightEdge.red;
				upperColor.green = alphaX * colorOnLeftEdge.green + betaX * colorOnRightEdge.green;
				upperColor.blue = alphaX * colorOnLeftEdge.blue + betaX * colorOnRightEdge.blue;

				lowerColor.red = alphaX * colorOnLowerEdge.red + betaX * colorOnLowerRightEdge.red;
				lowerColor.green = alphaX * colorOnLowerEdge.green + betaX * colorOnLowerRightEdge.green;
				lowerColor.blue = alphaX * colorOnLowerEdge.blue + betaX * colorOnLowerRightEdge.blue;

				float alphaY = 1 - (float)(y - leftTopY) / height;
				if (useCosine) {
					alphaY = MFPTR(m, alphaY);
				}
				float betaY = 1 - alphaY;

				finalColor.red = alphaY * upperColor.red + betaY * lowerColor.red;
				finalColor.green = alphaY * upperColor.green + betaY * lowerColor.green;
				finalColor.blue = alphaY * upperColor.blue + betaY * lowerColor.blue;

				SetPixel(x, y, finalColor);
			}
		}
		DrawCircle(leftTopX, leftTopY, 20, colorOnLeftEdge);
		DrawCircle(leftTopX + width, leftTopY, 20, colorOnRightEdge);
		DrawCircle(leftTopX, leftTopY + height, 20, colorOnLowerEdge);
		DrawCircle(leftTopX + width, leftTopY + height, 20, colorOnLowerRightEdge);
		if (GetKey('A').keyReleased) {
			useCosine = !useCosine;
		}
		if (useCosine) {
			DrawString("ON", 0, 0, 50, { 255,255,255 });
		}
		else {
			DrawString("OFF", 0, 0, 50, { 255,255,255 });
		}
	}
	float PerlinNoiseAttempt(int x,
		int y,
		int cellSize,
		int width,
		int height) {
		int remappedX = x / cellSize;
		int remappedY = y / cellSize;
		remappedX *= cellSize;
		remappedY *= cellSize;
		int leftTopX = remappedX;
		int leftTopY = remappedY;
		int outX;
		int outY;
		MapXandYToCellSpace(x, y, outX, outY, cellSize);
		int vectorsPerRow = (width / cellSize) + 1;
		int vectorTopLeft = outX + outY * vectorsPerRow;
		int vectorTopRight = (outX + 1) + outY * vectorsPerRow;
		int vectorLowerLeft = outX + (outY + 1) * vectorsPerRow;
		int vectorLowerRight = (outX + 1) + (outY + 1) * vectorsPerRow;

		BMath::Vec2 gVectorOnTopLeftEdge = this->gradientVecs[vectorTopLeft];
		gVectorOnTopLeftEdge.Normalize();

		BMath::Vec2 gVectorOnTopRightEdge = this->gradientVecs[vectorTopRight];
		gVectorOnTopRightEdge.Normalize();

		BMath::Vec2 gVectorOnLowerLeftEdge = this->gradientVecs[vectorLowerLeft];
		gVectorOnLowerLeftEdge.Normalize();

		BMath::Vec2 gVectorOnLowerRightEdge = this->gradientVecs[vectorLowerRight];
		gVectorOnLowerRightEdge.Normalize();

		BMath::Vec2 distanceFromTopLeftEdge = { x - leftTopX, y - leftTopY };
		distanceFromTopLeftEdge.Normalize();

		BMath::Vec2 distanceFromTopRightEdge = { x - (leftTopX + cellSize),y - leftTopY };
		distanceFromTopRightEdge.Normalize();

		BMath::Vec2 distanceFromLowerLeftEdge = { x - leftTopX,y - (leftTopY + cellSize) };
		distanceFromLowerLeftEdge.Normalize();

		BMath::Vec2 distanceFromLowerRightEdge = { x - (leftTopX + cellSize),y - (leftTopY + cellSize) };
		distanceFromLowerRightEdge.Normalize();


		float topLeftDot = distanceFromTopLeftEdge * gVectorOnTopLeftEdge;
		float topRightDot = distanceFromTopRightEdge * gVectorOnTopRightEdge;
		float lowerLeftDot = distanceFromLowerLeftEdge * gVectorOnLowerLeftEdge;
		float lowerRightDot = distanceFromLowerRightEdge * gVectorOnLowerRightEdge;
		float alphaX = 1 - (float)(x - leftTopX) / cellSize;
		alphaX = CosineInterpolation(alphaX);
		float betaX = 1 - alphaX;

		float firstFinalValue = alphaX * topLeftDot + betaX * topRightDot;
		float secondFinalValue = alphaX * lowerLeftDot + betaX * lowerRightDot;

		float alphaY = 1 - (float)(y - leftTopY) / cellSize;
		alphaY = CosineInterpolation(alphaY);
		float betaY = 1 - alphaY;

		float finalValue = alphaY * firstFinalValue + betaY * secondFinalValue;
		finalValue = (finalValue + 1.0f) / 2;
		finalValue = CosineInterpolation(finalValue);
		assert(finalValue <= 1 && finalValue >= 0);
		return finalValue;
	}
	//Size of cells -> XxY -> means if cell is 2 in width and 2 in height pass 2 to this function
	int GetNumberOfVectorsToCreate(int sizeOfCells, int width, int height) {
		int cellsInXDirection = width / sizeOfCells;
		int cellsInYDirection = height / sizeOfCells;
		return (cellsInXDirection + 1) * (cellsInYDirection + 1);
	}
	void MapXandYToCellSpace(int x, int y, int & outX, int & outY, int sizeOfCell) {
		outX = x / sizeOfCell;
		outY = y / sizeOfCell;
	}
	virtual bool OnCreate() override {
		int numberOfGraedientVectors = GetNumberOfVectorsToCreate(cellSizeL, screenHeight, screenWidth);
		for (int i = 0; i < numberOfGraedientVectors; i++) {
			this->gradientVecs.push_back({ (rand() % 30) - 15,(rand() % 30) - 15 });
		}
		for (int i = 0; i < 256; i++) {
			randomValues[i] = (float)rand()/RAND_MAX;
		}
		return true;
	}
	int GetNumbersOfCellsInXDirection(int sizeOfCell, int width) {
		return width / sizeOfCell;
	}
	void DrawPerlinNoise() {
		for (int i = 0; i < screenWidth; i++) {
			for (int j = 0; j < screenWidth; j++) {
				float perlinNoise = PerlinNoiseAttempt(i, j, cellSizeL, screenWidth, screenHeight);
				SetBlendingMode(ALPHA);
				SetPixel(i, j, { 255,255,128,(int)(255 * perlinNoise) });
				SetBlendingMode(NORMAL);
			}
		}
	}
	int nOctaves = 1;
	float PerlinNoise1D(int x)
	{

		float fNoise = 0.0f;
		float fScaleAcc = 0.0f;
		float fScale = 1.0f;

		for (int o = 0; o < nOctaves; o++)
		{
			int nPitch = 800 >> o;
			int nSample1 = (x / nPitch) * nPitch;
			int nSample2 = (nSample1 + nPitch) % 256;

			float fBlend = (float)(x - nSample1) / (float)nPitch;

			float fSample = (1.0f - fBlend) * randomValues[nSample1] + fBlend * randomValues[nSample2];

			fScaleAcc += fScale;
			fNoise += fSample * fScale;
			fScale = fScale / 2;
		}

		// Scale to seed range
		return (fNoise / fScaleAcc);
	}
	virtual bool OnUpdate(float elapsedTime) {
		ClearScreen(BColors::BLACK);
		//DrawPerlinNoise();
		for (int i = 0; i < 256; i++) {
			float t = (float)i / 256;
			float value = PerlinNoise1D(i) * 100;
			DrawLine(i, 300, i, 300 - value, BColors::MAROON);
		}
		if (GetKey(VK_SPACE).keyReleased) {
			nOctaves++;
			nOctaves = nOctaves % 10;
		}
		return true;
	}
};
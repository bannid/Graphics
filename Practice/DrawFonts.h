#pragma once
#include "BEngine.h"

class DrawFonts : public BEngine {
	
	virtual bool OnCreate()override {
		
		return true;
	}
	virtual bool OnUpdate(float elapsedTime) override {
		ClearScreen(WHITE);
		DrawString("QWERTYUIOPASDFGHJKLZXCVBNM1234567890:", 0, 0, 40, { 1.0f,0,0,0.5f });
		DrawString("QWERTYUIOPASDFGHJKLZXCVBNM1234567890:", 0, 40, 40, { 1.0f,0,0,0.5f });
		DrawString("QWERTYUIOPASDFGHJKLZXCVBNM1234567890:", 0, 80, 40, { 1.0f,0,0,0.5f });
		return true;
	}
};
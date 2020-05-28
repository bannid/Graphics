#pragma once
#include "BEngine.h"

class DrawFonts : public BEngine {
	
	virtual bool OnCreate()override {
		
		return true;
	}
	virtual bool OnUpdate(float elapsedTime) override {
		ClearScreen(BColors::WHITE);
		DrawString("QWERTYUIOPASDFGHJKLZXCVBNM1234567890:", 0, 0, 40, { 255,0,0,100 });
		DrawString("QWERTYUIOPASDFGHJKLZXCVBNM1234567890:", 0, 40, 40, { 255,0,0,100 });
		DrawString("QWERTYUIOPASDFGHJKLZXCVBNM1234567890:", 0, 80, 40, { 255,0,0,100 });
		return true;
	}
};
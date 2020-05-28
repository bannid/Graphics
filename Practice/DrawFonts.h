#pragma once
#include "BEngine.h"

class DrawFonts : public BEngine {
	
	virtual bool OnCreate()override {
		
		return true;
	}
	virtual bool OnUpdate(float elapsedTime) override {
		ClearScreen(BColors::WHITE);
		DrawString("Banni singh dhaliwal", 0, 100, 100, 0);
		return true;
	}
};
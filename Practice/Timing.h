#pragma once
#include "BEngine.h"
#include "Debug.h"
class Timing : public BEngine {
	virtual bool OnCreate() override {
		TIMED;
		return true;
	}
	virtual bool OnUpdate(float elapsedTime) override {
		TIMED;
		return true;
	}
};
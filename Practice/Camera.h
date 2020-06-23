#pragma once
#include "Maths.h"
#include "Common.h"
struct Camera {
	BMath::Vec4 forward = { 0.0f,0.0f,1.0f,0.0f };
	BMath::Vec4 position = { 0.0f,0.0f,0.0f,1.0f };
	float pitch;
	float yaw;
	void Yaw(float anlgeInDegrees);
	void Pitch(float angleInDegrees);
};
#pragma once
#include "Maths.h"
#include "Common.h"
#define MOUSE_SENSTIVITY 0.1f
struct Camera {
	BMath::Vec4 forward = { 0.0f,0.0f,1.0f,0.0f };
	BMath::Vec4 up = { 0.0f,1.0f,0.0f,0.0f };
	BMath::Vec4 right = { 1.0f,0.0f,0.0f,0.0f };
	BMath::Vec4 position = { 0.0f,0.0f,0.0f,1.0f };
	float speed = 20.0f;
	float pitch;
	float yaw;
	void Yaw(float anlgeInDegrees);
	void Pitch(float angleInDegrees);
};
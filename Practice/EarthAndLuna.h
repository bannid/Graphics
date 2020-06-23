#pragma once
/*
	Simple 3d graphics where moon is orbiting the earth.
*/
#include "BEngine3D.h"
#include "Mesh.h"
#include "Common.h"
#include "Utils.h"

using namespace BMath;
class EarthAndLuna : public BEngine3D {
	Mesh earth;
	Mesh luna;
	Vec4 lunaPos;
	Texture galaxy;
	int frame = 0;
	virtual bool OnCreate() override {
		earth = Mesh("C:\\Users\\Winny-Banni\\Desktop\\videos\\sphere.obj", 1000);
		luna = Mesh("C:\\Users\\Winny-Banni\\Desktop\\videos\\sphere.obj", 270);
		BUtils::LoadTexturePNG("C:\\Users\\Winny-Banni\\Pictures\\earth.png", &earth.tex);
		BUtils::LoadTexturePNG("C:\\Users\\Winny-Banni\\Pictures\\moon.png", &luna.tex);

		BUtils::LoadTexturePNG("C:\\Users\\Winny-Banni\\Pictures\\galaxy.png", &galaxy);

		earth.position.z = 12000;
		earth.position.x = 0;
		earth.position.y = 20.0f;
		earth.position.w = 1.0f;
		lunaPos = { 0.0f,0.0f,-5000.0f,1.0f };
		luna.position = earth.position - lunaPos;
		this->Initialise();
		return true;
	}

	virtual bool OnUpdate(float elapsedTime) override {

		ClearScreenWithTexture(&galaxy);
		ClearZBuffer();
		Draw(luna);
		Draw(earth);
		float rotationInDegrees = 1;
		luna.RotateY(rotationInDegrees);
		earth.RotateY(rotationInDegrees);
		Mat4 mat = BMath::RotationY(rotationInDegrees);
		lunaPos = lunaPos * mat;
		luna.position = earth.position - lunaPos;
		
		if (GetKey(VK_LBUTTON).keyDown) {
			if (std::abs(mouseDeltaX) > std::abs(mouseDeltaY)) {
				this->cam.Yaw(-mouseDeltaX * elapsedTime);
			}
			else {
				this->cam.Pitch(mouseDeltaY * elapsedTime);
			}
			
		}
		if (GetKey(VK_UP).keyDown) {
			this->cam.position = this->cam.position + (this->cam.forward * 10);
		}
		if (GetKey(VK_DOWN).keyDown) {
			this->cam.position = this->cam.position - (this->cam.forward * 10);
		}
		DrawVector(0, 0, 25, this->cam.right);
		DrawVector(150, 0, 25, this->cam.up);
		DrawVector(300, 0, 25, this->cam.forward);
		DrawVector(450, 0, 25, this->cam.position);
		
		DrawString(std::to_string(this->cam.pitch), 600, 0,25, { 1.0f,1.0f,1.0f });
		DrawString(std::to_string(this->cam.yaw), 750, 0, 25, { 1.0f,1.0f,1.0f });
		DrawString(std::to_string(this->cam.roll), 900, 0, 25, { 1.0f,1.0f,1.0f });
		frame++;
		return true;
	}
};
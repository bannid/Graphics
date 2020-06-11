#pragma once
/*
	Simple 3d graphics where moon is orbiting the earth.
*/
#include "BEngine3D.h"
#include "Mesh.h"
#include "Common.h"
using namespace BMath;
class EarthAndLuna : public BEngine3D {
	Mesh earth;
	Mesh luna;
	Vec4 lunaPos;
	virtual bool OnCreate() override {
		earth = Mesh("C:\\Users\\Winny-Banni\\Desktop\\videos\\sphere.obj", 1000);
		luna = Mesh("C:\\Users\\Winny-Banni\\Desktop\\videos\\sphere.obj", 27);
		LoadTexturePNG("C:\\Users\\Winny-Banni\\Pictures\\earth.png", &earth.tex, true);
		LoadTexturePNG("C:\\Users\\Winny-Banni\\Pictures\\moon.png", &luna.tex, true);
		earth.position.z = 5000;
		lunaPos = { 0.0f,0.0f,-5000.0f,1.0f };
		luna.position = earth.position - lunaPos;
		this->Initialise();
		return true;
	}

	virtual bool OnUpdate(float elapsedTime) override {
		ClearScreen(BColors::BLACK);
		ClearZBuffer();
		DrawMesh(luna);
		DrawMesh(earth);
		float rotationInDegrees = 1;
		luna.RotateY(rotationInDegrees);
		earth.RotateY(rotationInDegrees);
		Mat4 mat = BMath::RotationY(rotationInDegrees);
		lunaPos = lunaPos * mat;
		luna.position = earth.position - lunaPos;
		if (GetKey(VK_RIGHT).keyDown) {
			this->cam.Yaw(10 * elapsedTime);
		}
		else if(GetKey(VK_LEFT).keyDown){
			this->cam.Yaw(-10 * elapsedTime);
		}
		else if (GetKey(VK_UP).keyDown) {
			this->cam.Pitch(10 * elapsedTime);
		}
		else if (GetKey(VK_DOWN).keyDown) {
			this->cam.Pitch(-10 * elapsedTime);
		}
		if (GetKey(VK_LBUTTON).keyDown) {
			this->cam.position = this->cam.position + (this->cam.forward * 10);
		}
		else if (GetKey(VK_RBUTTON).keyDown) {
			this->cam.position = this->cam.position - (this->cam.forward * 10);
		}
		DrawVector(0, 0, 25, this->cam.right);
		DrawVector(150, 0, 25, this->cam.up);
		DrawVector(300, 0, 25, this->cam.forward);
		return true;
	}
};
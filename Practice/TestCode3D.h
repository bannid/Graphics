#pragma once
#include "BEngine3D.h"
#include "Mesh.h"
#include "Utils.h"
class TestCode3D : public BEngine3D {
	Mesh triangle;
	Mesh * earth;
	Mesh * moon;
	virtual bool OnCreate() override {
		earth = new Mesh("C:\\Users\\Winny-Banni\\Desktop\\videos\\sphere.obj",100);
		moon = new Mesh("C:\\Users\\Winny-Banni\\Desktop\\videos\\sphere.obj", 27);
		BUtils::LoadTexturePNG("C:\\Users\\Winny-Banni\\Pictures\\earth.png",&earth->tex);
		BUtils::LoadTexturePNG("C:\\Users\\Winny-Banni\\Pictures\\moon.png", &moon->tex);
		earth->position.x = 0;
		earth->position.y = 0;
		earth->position.z = 500;
		moon->position.x = 100;
		moon->position.y = 0;
		moon->position.z = 500;
		this->Initialise();
		return true;
	}

	virtual bool OnUpdate(float elapsedTime) override {
		ClearScreen(BLACK);
		ClearZBuffer();
		this->DrawMesh(*this->earth);
		this->DrawMesh(*this->moon);
		Mesh * selector = this->earth;
		if (GetKey(VK_UP).keyDown) {
			selector->position.z += 50.0f * elapsedTime;
		}
		if (GetKey(VK_DOWN).keyDown) {
			selector->position.z -= 50.0f * elapsedTime;
		}
		if (GetKey(VK_RIGHT).keyDown) {
			selector->position.x += 50.0f * elapsedTime;
		}
		if (GetKey(VK_LEFT).keyDown) {
			selector->position.x -= 50.0f * elapsedTime;
		}
		DrawVector(0, 0, 25, selector->position);
		return true;
	}
};
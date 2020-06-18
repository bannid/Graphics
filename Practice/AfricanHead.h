#pragma once
#include "BEngine3D.h"
#include "Mesh.h"
#include "Utils.h"

class AfricanHead : public BEngine3D {
	Mesh africanHead;
	BMath::Vec4 offset = { 0.0f,2.0f,5.0f,0.0f };
	bool rotate = true;
	virtual bool OnCreate() override {
		africanHead = Mesh("C:\\Users\\Winny-Banni\\Desktop\\videos\\african_head.obj", 1);
		BUtils::LoadTexturePNG("C:\\Users\\Winny-Banni\\Pictures\\african_head_diffuse.tga", &africanHead.tex);
		africanHead.position.z = 10.0f;
		africanHead.position.w = 1.0f;
		this->Initialise();
		return true;
	}

	virtual bool OnUpdate(float elapsedTime) override {
		ClearScreen(0);
		ClearZBuffer();
		if (GetKey(VK_SPACE).keyDown) {
			BMath::Mat4 mat = BMath::RotationY(1);
			offset = offset * mat;
		}
		if (GetKey('A').keyReleased) {
			BMath::Mat4 mat = BMath::RotationY(1);
			offset = offset * mat;
		}
		if (GetKey('W').keyReleased) {
			BMath::Mat4 mat = BMath::RotationY(-1);
			offset = offset * mat;
		}
		cam.LookAt(africanHead.position - offset, africanHead.position, { 0.0f,1.0f,0.0f,0.0f });
		DrawMesh(this->africanHead);
		if (GetKey(VK_DOWN).keyDown) {
			africanHead.position.z--;
		}
		if (GetKey(VK_UP).keyDown) {
			africanHead.position.z++;
		}
		DrawVector(0, 0, 25, cam.up);
		DrawVector(200, 0, 25, cam.right);
		DrawVector(400, 0, 25, cam.forward);
		DrawVector(0, 200, 25, cam.position);
		return true;
	}
};
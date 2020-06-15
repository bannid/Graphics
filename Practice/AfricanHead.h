#pragma once
#include "BEngine3D.h"
#include "Mesh.h"

class AfricanHead : public BEngine3D {
	Mesh africanHead;
	virtual bool OnCreate() override {
		africanHead = Mesh("C:\\Users\\Winny-Banni\\Desktop\\videos\\african_head.obj",1);
		LoadTexturePNG("C:\\Users\\Winny-Banni\\Desktop\\videos\\african_head_diffuse.png",&africanHead.tex,true);
		africanHead.position.z = 10.0f;
		africanHead.position.w = 1.0f;
		this->Initialise();
		return true;
	}

	virtual bool OnUpdate(float elapsedTime) override {
		ClearScreen(0);
		ClearZBuffer();
		DrawMesh(this->africanHead);
		if (GetKey(VK_DOWN).keyDown) {
			africanHead.position.z--;
		}
		if (GetKey(VK_UP).keyDown) {
			africanHead.position.z++;
		}
		return true;
	}
};
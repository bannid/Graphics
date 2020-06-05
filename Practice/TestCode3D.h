#pragma once
#include "BEngine.h"
#include "Mesh.h"

class TestCode3D : public BEngine {
	Mesh * mesh;
	virtual bool OnCreate() override {
		Mesh m("C:\\Users\\Winny-Banni\\Desktop\\videos\\african_head.obj");
		return true;
	}

	virtual bool OnUpdate(float elapsedTime) override {

		return true;
	}
};
#pragma once
#include "BEngine3D.h"
#include "Mesh.h"
#include "Utils.h"
#include "Shaders.h"
#include "Camera.h"

class AfricanHead : public BEngine3D {
	Mesh africanHead;
	Texture africanHeadTexture;
	Camera cam;
	BMath::Vec4 offset = { 0.0f,2.0f,5.0f,0.0f };
	bool rotate = true;
	void SetupShader(IShader * shader_) {
		Shader * shader = static_cast<Shader*>(shader_);
		shader->uniformModelMatrix = africanHead.GetModelMat();
		BMath::LookAt(cam.position, cam.position + cam.forward, { 0,1,0,0 }, shader->uniformViewMatrix);
		shader->uniformProjMatrix = BMath::PerspectiveProjection(50.0f, 100.0f, 1.0f, 1.0f);
		shader->uniformNormalMatrix = shader->uniformModelMatrix.Inverted().Transposed();
		shader->texture = &africanHeadTexture;
	}
	virtual bool OnCreate() override {
		africanHead = Mesh("C:\\Users\\Winny-Banni\\Desktop\\videos\\plane.obj", 10);
		BUtils::LoadTexturePNG("C:\\Users\\Winny-Banni\\Pictures\\container.jpg", &africanHeadTexture);
		africanHead.position.z = 50.0f;
		africanHead.position.w = 1.0f;
		cam.position = { 0,0,0,1 };
		cam.forward = { 0,0,1,0 };
		this->shader = new Shader();
		SetupShader(this->shader);
		return true;
	}

	virtual bool OnUpdate(float elapsedTime) override {
		ClearScreen(0);
		ClearZBuffer();
		Draw(this->africanHead);
		if (GetKey(VK_UP).keyDown) {
			cam.position = cam.position + (cam.forward * cam.speed * elapsedTime);
		}
		if (GetKey(VK_DOWN).keyDown) {
			cam.position = cam.position - (cam.forward * cam.speed * elapsedTime);
		}
		if (GetKey(VK_RIGHT).keyDown) {
			cam.position = cam.position + (cam.right * cam.speed * elapsedTime);
		}
		if (GetKey(VK_LEFT).keyDown) {
			cam.position = cam.position - (cam.right * cam.speed * elapsedTime);
		}
		cam.Yaw(mouseDeltaX);
		cam.Pitch(mouseDeltaY);
		BMath::LookAt(cam.position, cam.position + cam.forward, { 0,1,0,0 }, static_cast<Shader*>(this->shader)->uniformViewMatrix);
		return true;
	}
};
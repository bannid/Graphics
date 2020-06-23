#pragma once
#include "BEngine3D.h"
#include "Mesh.h"
#include "Utils.h"
#include "Shaders.h"
#include "Camera.h"

class Shader : public IShader {
public:
	BMath::Mat4 uniformProjMatrix;
	BMath::Mat4 uniformViewMatrix;
	BMath::Mat4 uniformModelMatrix;
	BMath::Mat4 uniformNormalMatrix;
	Texture * texture;
	virtual void VertexShader(Vertex * vertex) override {
		BMath::Mat4 M = uniformModelMatrix * uniformViewMatrix * uniformProjMatrix;
		vertex->normal = vertex->normal * uniformNormalMatrix;
		vertex->vector = vertex->vector * M;
	}
	virtual void FragmentShader(float alpha, float beta, float gamma,
		Vertex one,
		Vertex two,
		Vertex three,
		BColor & output) override {
		//Temp light
		LightDirectional light;
		light.direction = { 0, 0, -1, 0 };
		light.color = { 1.0f,1.0f,1.0f };
		one.normal.Normalize();
		two.normal.Normalize();
		three.normal.Normalize();
		float dotAlpha = one.normal * light.direction;
		float dotBeta = two.normal * light.direction;
		float dotGamma = three.normal * light.direction;

		float uvXAlpha = one.uv.x;
		float uvYAlpha = one.uv.y;

		float uvXBeta = two.uv.x;
		float uvYBeta = two.uv.y;

		float uvXGamma = three.uv.x;
		float uvYGamma = three.uv.y;

		float finalUVx = alpha * uvXAlpha + beta * uvXBeta + gamma * uvXGamma;
		float finalUVy = alpha * uvYAlpha + beta * uvYBeta + gamma * uvYGamma;
		output = BUtils::GetColorFromTexture(finalUVx, 1 - finalUVy, texture);
		float intensityFinal = dotAlpha * alpha + dotBeta * beta + dotGamma * gamma;
		output.red *= light.color.red;
		output.green *= light.color.green;
		output.blue *= light.color.blue;
		if (intensityFinal < 0.05) {
			intensityFinal = 0.05f;
		}
		output.red *= intensityFinal;
		output.green *= intensityFinal;
		output.blue *= intensityFinal;
	}
	Shader() {}
};

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
		africanHead = Mesh("C:\\Users\\Winny-Banni\\Desktop\\videos\\african_head.obj", 1);
		BUtils::LoadTexturePNG("C:\\Users\\Winny-Banni\\Pictures\\african_head_diffuse.tga", &africanHeadTexture);
		africanHead.position.z = 10.0f;
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
			cam.position.z+=cam.speed * elapsedTime;
		}
		if (GetKey(VK_DOWN).keyDown) {
			cam.position.z -= cam.speed * elapsedTime;
		}
		if (GetKey(VK_RIGHT).keyDown) {
			cam.position.x += cam.speed * elapsedTime;
		}
		if (GetKey(VK_LEFT).keyDown) {
			cam.position.x -= cam.speed * elapsedTime;
		}
		cam.Yaw(mouseDeltaX * 0.1);
		BMath::LookAt(cam.position, cam.position + cam.forward, { 0,1,0,0 }, static_cast<Shader*>(this->shader)->uniformViewMatrix);
		return true;
	}
};
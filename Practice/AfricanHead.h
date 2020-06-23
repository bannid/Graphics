#pragma once
#include "BEngine3D.h"
#include "Mesh.h"
#include "Utils.h"
#include "Shaders.h"

class Shader : public IShader {
public:
	BMath::Mat4 uniformProjMatrix;
	BMath::Mat4 uniformViewMatrix;
	BMath::Mat4 uniformModelMatrix;
	BMath::Mat4 uniformViewPortMatrix;
	Texture * texture;
	virtual void VertexShader(std::vector<Vertex> & vertices) override {
		BMath::Mat4 M = uniformModelMatrix * uniformViewMatrix * uniformProjMatrix * uniformViewPortMatrix;
		for (auto it = vertices.begin(); it != vertices.end(); it++) {
			auto normalMatrx = uniformModelMatrix.Inverted().Transposed();
			it->normal = it->normal * normalMatrx;
			it->vector = it->vector * M;
		}
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
	Shader(){}
};

class AfricanHead : public BEngine3D {
	Mesh africanHead;
	BMath::Vec4 offset = { 0.0f,2.0f,5.0f,0.0f };
	bool rotate = true;
	void SetupShader(IShader * shader_) {
		Shader * shader = static_cast<Shader*>(shader_);
		shader->uniformModelMatrix = africanHead.GetModelMat();
		BMath::LookAt(cam.position, cam.position + cam.forward, { 0,1,0,0 }, shader->uniformViewMatrix);
		shader->uniformProjMatrix = BMath::PerspectiveProjection(50.0f,100.0f,1.0f,1.0f);

		auto & viewPortMatrix = shader->uniformViewPortMatrix;
		int screenHeight = GetScreenHeight();
		int screenWidth = GetScreenWidth();
		viewPortMatrix.m[0][0] = screenWidth / 2.0f;
		viewPortMatrix.m[1][1] = screenHeight / 2.0f;
		viewPortMatrix.m[3][0] = screenWidth / 2;
		viewPortMatrix.m[3][1] = screenHeight / 2;
		shader->texture = &africanHead.tex;
	}
	virtual bool OnCreate() override {
		africanHead = Mesh("C:\\Users\\Winny-Banni\\Desktop\\videos\\african_head.obj", 1);
		BUtils::LoadTexturePNG("C:\\Users\\Winny-Banni\\Pictures\\african_head_diffuse.tga", &africanHead.tex);
		africanHead.position.z = 10.0f;
		africanHead.position.w = 1.0f;
		this->shader = new Shader();
		SetupShader(this->shader);
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
		DrawMesh(this->africanHead);
		return true;
	}
};
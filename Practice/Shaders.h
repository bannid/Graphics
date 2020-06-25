#pragma once
#include <vector>
#include "Common.h"
#include "Utils.h"
class IShader {
public:
	virtual void VertexShader(Vertex * vertex) = 0;
	virtual void FragmentShader(float alpha,
		float beta,
		float gamma,
		Vertex one,
		Vertex two,
		Vertex three,
		BColor & output) = 0;
};
class Shader : public IShader {
public:
	BMath::Mat4 uniformProjMatrix;
	BMath::Mat4 uniformViewMatrix;
	BMath::Mat4 uniformModelMatrix;
	BMath::Mat4 uniformNormalMatrix;
	Texture * texture;
	virtual void VertexShader(Vertex * vertex) override;
	virtual void FragmentShader(float alpha, float beta, float gamma,
		Vertex one,
		Vertex two,
		Vertex three,
		BColor & output) override;
	Shader() {}
};
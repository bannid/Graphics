#include "Shaders.h"
void Shader::FragmentShader(float alpha, float beta, float gamma,
	Vertex one,
	Vertex two,
	Vertex three,
	BColor & output) {
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
	if (intensityFinal < 0.5) {
		intensityFinal = 0.5f;
	}
	output.red *= intensityFinal;
	output.green *= intensityFinal;
	output.blue *= intensityFinal;
}

void Shader::VertexShader(Vertex * vertex) {
	BMath::Mat4 M = uniformModelMatrix * uniformViewMatrix * uniformProjMatrix;
	vertex->normal = vertex->normal * uniformNormalMatrix;
	vertex->vector = vertex->vector * M;
}
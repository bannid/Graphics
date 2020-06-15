#include "Shaders.h"
#include "Mesh.h"
static void ScaleColor(float t, BColors::color_t & color) {
	if (t < 0.05) {
		t = 0.05f;
	}
	color.red *= t;
	color.green *= t;
	color.blue *= t;
}
void FlatShader::GetColor(Triangle & t, float alpha, float beta, float gamma, Mesh & mesh, int & output) {
	//TODO;
}
void FlatShader::GetColor(Triangle & t,
	float alpha, float beta, float gamma,
	Mesh & mesh,
	BColors::color_t & output) {
	//TODO;
	output = { 255,255,255,255 };
}
void GourardShaderTextured::GetColor(Triangle & t,
	float alpha, float beta, float gamma,
	Mesh & mesh,
	BColors::color_t & output) {
	BMath::Vec4 lightDir = { 0,0,-1,0 };
	lightDir.Normalize();
	t.vertices[0].normal.Normalize();
	t.vertices[1].normal.Normalize();
	t.vertices[2].normal.Normalize();
	float dotAlpha = t.vertices[0].normal * lightDir;
	float dotBeta = t.vertices[1].normal * lightDir;
	float dotGamma = t.vertices[2].normal * lightDir;

	float uvXAlpha = t.vertices[0].uv.x;
	float uvYAlpha = t.vertices[0].uv.y;

	float uvXBeta = t.vertices[1].uv.x;
	float uvYBeta = t.vertices[1].uv.y;

	float uvXGamma = t.vertices[2].uv.x;
	float uvYGamma = t.vertices[2].uv.y;

	float finalUVx = alpha * uvXAlpha + beta * uvXBeta + gamma * uvXGamma;
	float finalUVy = alpha * uvYAlpha + beta * uvYBeta + gamma * uvYGamma;
	BColors::color_t finalColor = BUtils::GetColorFromTexture(finalUVx, 1 - finalUVy, &mesh.tex);
	float intensityFinal = dotAlpha * alpha + dotBeta * beta + dotGamma * gamma;
	ScaleColor(intensityFinal, finalColor);
	output = finalColor;
}
void GourardShaderTextured::GetColor(Triangle & t,
	float alpha, float beta, float gamma,
	Mesh & mesh,
	int & output) {
	//TODO:
	output = 0xFFFFFFFF;
}
void GourardShader::GetColor(Triangle & t,
	float alpha, float beta, float gamma,
	Mesh & mesh,
	int & output) {
	//TODO:
	output = 0xFFFFFFFF;
}
void GourardShader::GetColor(Triangle & t,
	float alpha, float beta, float gamma,
	Mesh & mesh,
	BColors::color_t & output) {
	BMath::Vec4 lightDir = { 0,-1,0,0 };
	lightDir.Normalize();
	t.vertices[0].normal.Normalize();
	t.vertices[1].normal.Normalize();
	t.vertices[2].normal.Normalize();
	float dotAlpha = t.vertices[0].normal * lightDir;
	float dotBeta = t.vertices[1].normal * lightDir;
	float dotGamma = t.vertices[2].normal * lightDir;

	float uvXAlpha = t.vertices[0].uv.x;
	float uvYAlpha = t.vertices[0].uv.y;

	float uvXBeta = t.vertices[1].uv.x;
	float uvYBeta = t.vertices[1].uv.y;

	float uvXGamma = t.vertices[2].uv.x;
	float uvYGamma = t.vertices[2].uv.y;

	BColors::color_t finalColor = { 255,255,255 };
	float intensityFinal = dotAlpha * alpha + dotBeta * beta + dotGamma * gamma;
	ScaleColor(intensityFinal, finalColor);
	output = finalColor;
}
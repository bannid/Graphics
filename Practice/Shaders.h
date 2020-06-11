#pragma once
#include "Common.h"
#include "Utils.h"
#include "Maths.h"
#include "Mesh.h"
#include "BEngine.h"
struct Mesh;
class IShader {
public:
	virtual void GetColor(Triangle & t,float alpha, float beta, float gamma,Mesh & mesh,int & output) = 0;
	virtual void GetColor(Triangle & t,float alpha, float beta, float gamma,Mesh & mesh,BColors::color_t & output) = 0;
};

class FlatShader : public IShader {
public:
	virtual void GetColor(Triangle & t, float alpha, float beta, float gamma, Mesh & mesh, int & output) override;
	virtual void GetColor(Triangle & t,
		float alpha, float beta, float gamma,
		Mesh & mesh,
		BColors::color_t & output) override;
};
class GourardShaderTextured : public IShader {
public:
	virtual void GetColor(Triangle & t,
		float alpha, float beta, float gamma,
		Mesh & mesh,
		int & output) override;
	virtual void GetColor(Triangle & t,
		float alpha, float beta, float gamma,
		Mesh & mesh,
		BColors::color_t & output);
};

class GourardShader : public IShader {
public:
	virtual void GetColor(Triangle & t,
		float alpha, float beta, float gamma,
		Mesh & mesh,
		int & output) override;
	virtual void GetColor(Triangle & t,
		float alpha, float beta, float gamma,
		Mesh & mesh,
		BColors::color_t & output);
};
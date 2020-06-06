#pragma once
#include "BEngine.h"
#include "Maths.h"
#define RAD_TO_DEGREE(x) 180.0f/M_PI
class BEngine3D : public BEngine {
private:
	BMath::Mat4 viewPortMatrix;
	BMath::Mat4 projectionMatrix;
	BMath::Mat4 toNDC;
	float fov = RAD_TO_DEGREE(1.0f);
	bool initialised = false;
public:
	void DrawMesh(Mesh & mesh);
	void Initialise();
	void FillTriangleBC(Triangle & t);
};
#pragma once
#include "BEngine.h"
#include "Maths.h"
#include "Common.h"

class BEngine3D : public BEngine {
private:
	BMath::Mat4 viewPortMatrix;
	BMath::Mat4 projectionMatrix;
	float fov = 75.0f;
	float zNear = 1.0f;
	float zFar = 100.0f;
	bool initialised = false;
	
public:
	void DrawMesh(Mesh & mesh);
	void Initialise();
	void SetProjectionMatrix();
	void SetViewportMatrix();
	void FillTriangleBC(Triangle & t, Texture * tex);
};
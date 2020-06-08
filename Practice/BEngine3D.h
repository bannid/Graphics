#pragma once
#include "BEngine.h"
#include "Maths.h"
#include "Common.h"

class BEngine3D : public BEngine {
private:
	BMath::Mat4 viewPortMatrix;
	BMath::Mat4 projectionMatrix;
	BMath::Mat4 NDC;
	float fov = 85.0f;
	float zNear = 1.0f;
	float zFar = 100.0f;
	float screenWidth = 1000.0f;
	float screenHeight = 1000.0f;
	bool initialised = false;
public:
	void DrawMesh(Mesh & mesh);
	void Initialise();
	void SetProjectionMatrix();
	void SetViewportMatrix();
	void SetNDCMatrix();
	void FillTriangleBC(Triangle & t, Texture * tex);
};
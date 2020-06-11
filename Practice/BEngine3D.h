#pragma once
#include "BEngine.h"
#include "Maths.h"
#include "Common.h"

struct Camera {
	BMath::Vec4 up = { 0.0f,1.0f,0.0f,0.0f };
	BMath::Vec4 right = { 1.0f,0.0f,0.0f,0.0f };
	BMath::Vec4 forward = { 0.0f,0.0f,1.0f,0.0f };
	BMath::Vec4 position = { 0.0f,0.0f,0.0f,1.0f };
	BMath::Mat4 GetViewMatrix();
	void Yaw(float anlgeInDegrees);
	void Pitch(float angleInDegrees);
	void Roll(float angleInDegrees);
};
class BEngine3D : public BEngine {
private:
	BMath::Mat4 viewPortMatrix;
	BMath::Mat4 projectionMatrix;
	float fov = 60.0f;
	float zNear = 1.0f;
	float zFar = 100.0f;
	bool initialised = false;
	
public:
	Camera cam;
	void DrawMesh(Mesh & mesh);
	void Initialise();
	void SetProjectionMatrix();
	void SetViewportMatrix();
	void FillTriangleBC(Triangle & t, Mesh & mesh);
};
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
	float pitch;
	float yaw;
	float roll;
	void Yaw(float anlgeInDegrees);
	void Pitch(float angleInDegrees);
	void LookAt(BMath::Vec4 pos, BMath::Vec4 target, BMath::Vec4 up);
};
class BEngine3D : public BEngine {
private:
	BMath::Mat4 viewPortMatrix;
	BMath::Mat4 projectionMatrix;
	float fov = 50.0f;
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
	void FillTriangleBC(Vertex one, Vertex two, Vertex three, Mesh & mesh);
	//Settings
	bool doLighting = false;
	bool drawWireframe = false;
	bool drawWireframeOnly = false;
};
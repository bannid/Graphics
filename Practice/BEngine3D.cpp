#include "BEngine3D.h"

void BEngine3D::DrawMesh(Mesh & mesh) {
	assert(this->initialised);
	BMath::Mat4 modelMat = mesh.GetModelMat();
	BMath::Vec4 lightDir = { 0,0,1,0 };
	for (auto it = mesh.triangles.begin(); it != mesh.triangles.end(); it++) {
		Triangle t = *it;
		float zInverse = 1.0f / mesh.position.z;
		projectionMatrix.m[0][0] = zInverse;
		projectionMatrix.m[1][1] = zInverse;
		projectionMatrix.m[2][2] = zInverse;
		
		t.vertices[0].color = IntToColor(BColors::WHITE);
		BMath::Vec4 vec1 = t.vertices[1].vector - t.vertices[0].vector;
		BMath::Vec4 vec2 = t.vertices[2].vector - t.vertices[0].vector;
		BMath::Vec4 normal = vec1.Cross(vec2).Normalized();
		float intensity = normal * lightDir;
		if (intensity < 0)continue;
		t.vertices[0].color.red *= intensity;
		t.vertices[0].color.green *= intensity;
		t.vertices[0].color.blue *= intensity;

		t.vertices[0].vector = t.vertices[0].vector * modelMat;
		t.vertices[1].vector = t.vertices[1].vector * modelMat;
		t.vertices[2].vector = t.vertices[2].vector * modelMat;

		t.vertices[0].vector = t.vertices[0].vector * this->projectionMatrix;
		t.vertices[1].vector = t.vertices[1].vector * this->projectionMatrix;
		t.vertices[2].vector = t.vertices[2].vector * this->projectionMatrix;
		
		t.vertices[0].vector = t.vertices[0].vector * this->viewPortMatrix;
		t.vertices[1].vector = t.vertices[1].vector * this->viewPortMatrix;
		t.vertices[2].vector = t.vertices[2].vector * this->viewPortMatrix;
		
		FillTriangle(t);
	}
}

void BEngine3D::Initialise() {
	float screenHeight = GetScreenHeight();
	float screenWidth = GetScreenWidth();
	BMath::Mat4 mat;
	float aspectRatio = screenHeight / screenWidth;
	mat.m[0][0] = (screenWidth / 2.0f) * aspectRatio;
	mat.m[1][1] = (screenHeight / 2.0f);
	mat.m[3][0] = screenWidth / 2;
	mat.m[3][1] = screenHeight / 2;
	this->viewPortMatrix = mat;
	this->initialised = true;
}
#include "BEngine3D.h"

void BEngine3D::DrawMesh(Mesh & mesh) {
	assert(this->initialised);
	BMath::Mat4 modelMat = mesh.GetModelMat();
	BMath::Vec4 lightDir = { 0,0,-1,0 };
	//This is not the real screen.
	float screenDimensions = 1000.0f;
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
		this->toNDC.m[0][0] = 1.0f / screenDimensions;
		this->toNDC.m[1][1] = 1.0f / screenDimensions;
		BMath::Mat4 M = modelMat * projectionMatrix * toNDC * viewPortMatrix;

		t.vertices[0].vector = t.vertices[0].vector * M;
		t.vertices[1].vector = t.vertices[1].vector * M;
		t.vertices[2].vector = t.vertices[2].vector * M;
		
		FillTriangleBC(t);
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

float Min(float a, float b) {
	return a < b ? a : b;
}
float Max(float a, float b) {

	return a > b ? a : b;
}
void BEngine3D::FillTriangleBC(Triangle & t) {
	float minX = Min(t.vertices[0].vector.x, Min(t.vertices[1].vector.x,t.vertices[2].vector.x));
	float minY = Min(t.vertices[0].vector.y, Min(t.vertices[1].vector.y, t.vertices[2].vector.y));
	float maxX = Max(t.vertices[0].vector.x, Max(t.vertices[1].vector.x, t.vertices[2].vector.x));
	float maxY = Max(t.vertices[0].vector.y, Max(t.vertices[1].vector.y, t.vertices[2].vector.y));
	float mainTriangleArea = t.Area();
	for (int x = minX; x < maxX; x++) {
		for (int y = minY; y < maxY; y++) {
			BMath::Vec2 point = { x,y };
			BMath::Vec2 pointA = { t.vertices[0].vector.x,t.vertices[0].vector.y };
			BMath::Vec2 pointB= { t.vertices[1].vector.x,t.vertices[1].vector.y };
			BMath::Vec2 pointC = { t.vertices[2].vector.x,t.vertices[2].vector.y };
			BMath::Vec4 first(pointC.x - pointA.x, pointB.x - pointA.x, pointA.x - point.x,1.0f);
			BMath::Vec4 second(pointC.y - pointA.y, pointB.y - pointA.y, pointA.y - point.y, 1.0f);
			BMath::Vec4 cross = first.Cross(second);
			float Gamma = cross.y / cross.z;
			float Beta = cross.x / cross.z;
			float Alpha = 1.f - (cross.x + cross.y) / cross.z;
			float value = 0;
			if (Alpha > value &&
				Beta > value &&
				Gamma > value) {
				SetPixel(x, y, t.vertices[0].color);
			}
		}
	}
}

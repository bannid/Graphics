#pragma once
#include <vector>
#include "Common.h"
#include "Maths.h"
#include "BEngine.h"

struct Mesh {
	float size;
	std::vector<BMath::Vec4> vertices;
	std::vector<BMath::Vec4> normals;
	std::vector<BMath::Vec4> textureCoords;
	std::vector<int[3]> faces;
	Texture tex;
	std::vector<Triangle> triangles;
	BMath::Vec4 position;
	BMath::Vec4 up = { 0,1,0,0 };
	BMath::Vec4 right{ 1,0,0,0 };
	BMath::Vec4 forward{ 0,0,1,0 };
	Mesh(const char * objFile, float size);
	Mesh(float size);
	Mesh();
	bool LoadFromObjFile(const char * fileName);
	BMath::Mat4 GetModelMat();
	void ApplyRotationMatrix(BMath::Mat4 & mat);
	void RotateX(float angleInDegree);
	void RotateY(float angleInDegree);
	void RotateZ(float angleInDegree);
};
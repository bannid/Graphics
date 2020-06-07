#pragma once
#include <vector>
#include "Common.h"
#include "Maths.h"
#include "BEngine.h"

struct Mesh {
	float size = 500;
	std::vector<BMath::Vec4> vertices;
	std::vector<BMath::Vec4> normals;
	std::vector<BMath::Vec4> textureCoords;
	std::vector<int[3]> faces;
	Texture tex;
	std::vector<Triangle> triangles;
	BMath::Vec4 position;
	Mesh(const char * objFile);
	Mesh(){}
	bool LoadFromObjFile(const char * fileName);
	BMath::Mat4 GetModelMat();
};
#pragma once
#include <vector>
#include "Common.h"
#include "Maths.h"
struct Vertex {
	BMath::Vec4 vector;
	BColors::color_t color;
	BMath::Vec4 normal;
	BMath::Vec4 uv;
};
struct Triangle {
	Vertex vertices[3];
	Triangle(){}
};
struct Mesh {
	float size = 500;
	std::vector<BMath::Vec4> vertices;
	std::vector<BMath::Vec4> normals;
	std::vector<BMath::Vec4> textureCoords;
	std::vector<int[3]> faces;
	std::vector<Triangle> triangles;
	BMath::Vec4 position;
	Mesh(const char * objFile);
	Mesh(){}
	bool LoadFromObjFile(const char * fileName);
	BMath::Mat4 GetModelMat();
};
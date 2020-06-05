#pragma once
#include <vector>
#include "Polygon.h"
#include "Maths.h"
struct Vertex {
	BMath::Vec4 vector;
	BColors::color_t color;
	BMath::Vec4 normal;
	BMath::Vec4 uv;
};
struct Triangle {
	BMath::Vec4 v1;
	BMath::Vec4 v2;
	BMath::Vec4 v3;
	Vertex vertices[3];
	Triangle(){}
	Triangle(BMath::Vec4 v1, BMath::Vec4 v2, BMath::Vec4 v3) :v1(v1), v2(v2), v3(v3) {}
};
struct Mesh {
	std::vector<BMath::Vec4> vertices;
	std::vector<BMath::Vec4> normals;
	std::vector<BMath::Vec4> textureCoords;
	std::vector<int[3]> faces;
	std::vector<Triangle> triangles;
	BMath::Mat4 modelMatrix;
	Mesh(const char * objFile);
};
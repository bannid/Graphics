#pragma once
#include "BEngine.h"
#include "Common.h"
#include "Shaders.h"

class BEngine3D : public BEngine {
	CULL cullMode = NO_CULL;
public:
	IShader * shader;
	void Draw(Mesh & mesh);
	void FillTriangleBC(Vertex one, Vertex two, Vertex three, Mesh & mesh);
	void ClipAndDehomogniseVertices(std::vector<Triangle> & triangles, std::vector<Triangle> & output);
	void ClipVertices(Triangle & t, std::vector<Triangle> & output, Plane & plane);
};
#pragma once
#include "BEngine.h"
#include "Common.h"
#include "Shaders.h"

class BEngine3D : public BEngine {
public:
	IShader * shader;
	void Draw(Mesh & mesh);
	void FillTriangleBC(Vertex one, Vertex two, Vertex three, Mesh & mesh);
};
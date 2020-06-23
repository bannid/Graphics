#pragma once
#include "BEngine.h"
#include "Maths.h"
#include "Common.h"
#include "Shaders.h"
#include "Camera.h"

class BEngine3D : public BEngine {
private:
	bool initialised = false;
	
public:
	IShader * shader;
	Camera cam;
	void DrawMesh(Mesh & mesh);
	void FillTriangleBC(Vertex one, Vertex two, Vertex three, Mesh & mesh);
	//Settings
	bool doLighting = false;
	bool drawWireframe = false;
	bool drawWireframeOnly = false;
};
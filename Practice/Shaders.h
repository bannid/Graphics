#pragma once
#include <vector>
#include "Common.h"
class IShader {
public:
	virtual void VertexShader(std::vector<Vertex> & vertices) = 0;
	virtual void FragmentShader(float alpha,
		float beta,
		float gamma,
		Vertex one,
		Vertex two,
		Vertex three,
		BColor & output) = 0;
};
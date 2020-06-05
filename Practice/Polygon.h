#pragma once
#include "Maths.h"
#include "Common.h"
namespace BPolygon {
	struct Vertex {
		BMath::Vec4 vector;
		BMath::Vec2 uv;
		BColors::color_t color;
		BMath::Vec4 Normal;
	};
	struct Triangle {
		BMath::Vec4 v1;
		BMath::Vec4 v2;
		BMath::Vec4 v3;
		Triangle(BMath::Vec4 v1, BMath::Vec4 v2, BMath::Vec4 v3) :v1(v1), v2(v2), v3(v3) {}
	};
}

#pragma once
#include "Maths.h"
namespace BPolygon {
	struct Triangle {
		BMath::Vec4 v1;
		BMath::Vec4 v2;
		BMath::Vec4 v3;
		Triangle(BMath::Vec4 v1, BMath::Vec4 v2, BMath::Vec4 v3) :v1(v1), v2(v2), v3(v3) {}
	};
}

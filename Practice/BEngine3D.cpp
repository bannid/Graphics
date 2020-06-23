#include "BEngine3D.h"
#include "Utils.h"
void BEngine3D::Draw(Mesh & mesh) {
	auto vertices = mesh.vertexes;
	int screenHeightHalf = GetScreenHeight() / 2;
	int screenWidthHalf = GetScreenWidth() / 2;
	for (int i = 0; i < vertices.size(); i++) {
		this->shader->VertexShader(&vertices[i]);
		if (vertices[i].vector.w != 0.0f) {
			vertices[i].vector.x /= vertices[i].vector.w;
			vertices[i].vector.y /= vertices[i].vector.w;
			vertices[i].vector.z /= vertices[i].vector.w;
			vertices[i].vector.w /= vertices[i].vector.w;
		}
		//Translation to viewport
		vertices[i].vector.x *= screenWidthHalf;
		vertices[i].vector.y *= screenHeightHalf;
		vertices[i].vector.x += screenWidthHalf;
		vertices[i].vector.y += screenHeightHalf;
	}
	for (int i = 0; i < mesh.indices.size(); i += 3) {
		FillTriangleBC(vertices[i], vertices[i + 1], vertices[i + 2], mesh);
	}
}

void BEngine3D::FillTriangleBC(Vertex one, Vertex two, Vertex three, Mesh & mesh) {
	float minX = BUtils::Min(one.vector.x, BUtils::Min(two.vector.x, three.vector.x));
	float minY = BUtils::Min(one.vector.y, BUtils::Min(two.vector.y, three.vector.y));
	float maxX = BUtils::Max(one.vector.x, BUtils::Max(two.vector.x, three.vector.x));
	float maxY = BUtils::Max(one.vector.y, BUtils::Max(two.vector.y, three.vector.y));
	//Clamp to screen
	minX = BUtils::Max(0, minX);
	minY = BUtils::Max(0, minY);
	maxX = BUtils::Min(GetScreenWidth(), maxX);
	maxY = BUtils::Min(GetScreenHeight(), maxY);

	BMath::Vec2 pointA = { one.vector.x,one.vector.y };
	BMath::Vec2 pointB = { two.vector.x,two.vector.y };
	BMath::Vec2 pointC = { three.vector.x,three.vector.y };
	BMath::Vec2 edge1 = pointB - pointA;
	BMath::Vec2 edge2 = pointC - pointA;
	BMath::Vec2 edge3 = pointA - pointC;
	BMath::Vec2 edge4 = pointC - pointB;

	float mainTriangleArea = std::abs((edge1.x * edge2.y - edge1.y * edge2.x) * 0.5f);
	if (mainTriangleArea == 0)return;
	for (int x = minX; x < maxX; x++) {
		for (int y = minY; y < maxY; y++) {
			BMath::Vec2 point = { x,y };
			float alpha, beta, gamma;
			BMath::Vec2 vp1 = point - pointA;
			gamma = (edge1.x * vp1.y - edge1.y * vp1.x) * 0.5f;
			gamma /= mainTriangleArea;
			BMath::Vec2 vp2 = point - pointC;
			beta = (edge3.x * vp2.y - edge3.y * vp2.x) * 0.5;
			beta /= mainTriangleArea;
			BMath::Vec2 vp3 = point - pointB;
			alpha = (edge4.x * vp3.y - edge4.y * vp3.x) * 0.5;
			alpha /= mainTriangleArea;
			//Error threshold
			float value = 0 - 0.01;
			if (alpha > value && beta > value && gamma > value) {
				BColor finalColor;
				this->shader->FragmentShader(alpha, beta, gamma, one, two, three, finalColor);
				float zBuffer = GetZBuffer(x, y);
				float finalZ = alpha * one.vector.z + beta * two.vector.z + gamma * three.vector.z;
				if (zBuffer > finalZ) {
					SetPixel(x, y, finalColor);
					SetZBuffer(x, y, finalZ);
				}
			}
		}
	}
}

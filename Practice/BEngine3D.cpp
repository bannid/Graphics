#include "BEngine3D.h"
#include "Utils.h"
void BEngine3D::Draw(Mesh & mesh) {
	std::vector<Vertex> vertices = mesh.vertexes;
	std::vector<int> indices = mesh.indices;
	for (int i = 0; i < vertices.size(); i++) {
		this->shader->VertexShader(&vertices[i]);
	}
	std::vector<Triangle> triangles;
	std::vector<Triangle> clippedTriangles;
	for (int i = 0; i < indices.size(); i += 3) {
		Triangle t(vertices[i], vertices[i + 1], vertices[i + 2]);
		triangles.push_back(t);
	}
	ClipAndDehomogniseVertices(triangles, clippedTriangles);
	for (int i = 0; i < clippedTriangles.size(); i++) {
		FillTriangleBC(clippedTriangles[i].one, clippedTriangles[i].two, clippedTriangles[i].three, mesh);
		if (GetKey(VK_SPACE).keyDown) {
			FillTriangle(clippedTriangles[i]);
		}
	}
}
void Dehomoginise(Vertex & vertex) {
	if (vertex.vector.w != 0.0f) {
		vertex.vector.x /= vertex.vector.w;
		vertex.vector.y /= vertex.vector.w;
		vertex.vector.z /= vertex.vector.w;
		vertex.vector.w /= vertex.vector.w;
	}
}
void ScaleToViewport(Vertex & vertex, int x, int y) {
	vertex.vector.x *= x;
	vertex.vector.y *= y;
	vertex.vector.x += x;
	vertex.vector.y += y;
}
void ClipTwoVertcesOut(Vertex & outA, Vertex & outB, Vertex & inside,
	Vertex & newA, Vertex & newB, Plane & plane) {
	Line fromInsideToA;
	fromInsideToA.origin = inside.vector;
	fromInsideToA.direction = (outA.vector - inside.vector).Normalized();
	float tForNewPoint1 = plane.IntersectWithLine(fromInsideToA);
	BMath::Vec4 newPointA = inside.vector + ((fromInsideToA.direction) * std::abs(tForNewPoint1));

	Line fromInsideToB;
	fromInsideToB.origin = inside.vector;
	fromInsideToB.direction = (outB.vector - inside.vector).Normalized();
	float tForNewPoint2 = plane.IntersectWithLine(fromInsideToB);
	BMath::Vec4 newPointb = inside.vector + ((fromInsideToB.direction) * std::abs(tForNewPoint2));
	newA = outA;
	newB = outB;
	newA.vector = newPointA;
	newB.vector = newPointb;
	float alpha = (newA.vector - inside.vector).Magnitude() / (outA.vector - inside.vector).Magnitude();
	float beta = 1.0f - alpha;
	//Interpolate w
	newA.vector.w = alpha * outA.vector.w + beta * inside.vector.w;
	//Interpolate uv coords
	newA.uv.x = inside.uv.x + alpha * (outA.uv.x - inside.uv.x);
	newA.uv.y = inside.uv.y + alpha * (outA.uv.y - inside.uv.y);

	alpha = (newB.vector - inside.vector).Magnitude() / (outB.vector - inside.vector).Magnitude();
	beta = 1.0f - alpha;
	newB.vector.w = alpha * outB.vector.w + beta * inside.vector.w;
	newB.uv.x = inside.uv.x + alpha * (outB.uv.x - inside.uv.x);
	newB.uv.y = inside.uv.y + alpha * (outB.uv.y - inside.uv.y);
}
void ClipOneVerticeOut(Vertex & outside, Vertex & insideA, Vertex & insideB, Vertex & newA, Vertex & newB,
	Plane & plane) {
	Line fromPointA;
	fromPointA.origin = insideA.vector;
	fromPointA.direction = (outside.vector - insideA.vector).Normalized();
	float tForNewPoint1 = plane.IntersectWithLine(fromPointA);
	BMath::Vec4 newPointA = insideA.vector + ((fromPointA.direction) * std::abs(tForNewPoint1));

	Line fromPointB;
	fromPointB.origin = insideB.vector;
	fromPointB.direction = (outside.vector - insideB.vector).Normalized();
	float tForNewPoint2 = plane.IntersectWithLine(fromPointB);

	BMath::Vec4 newPointB = insideB.vector + ((fromPointB.direction) * std::abs(tForNewPoint2));
	newA = outside;
	newB = outside;
	newA.vector = newPointA;
	newB.vector = newPointB;
	
	float alpha = (newA.vector - insideA.vector).Magnitude() / (outside.vector - insideA.vector).Magnitude();
	float beta = 1.0f - alpha;

	newA.vector.w = outside.vector.w * alpha + beta * insideA.vector.w;
	newA.uv.x = insideA.uv.x + alpha * (outside.uv.x - insideA.uv.x);
	newA.uv.y = insideA.uv.y + alpha * (outside.uv.y - insideA.uv.y);

	alpha = (newB.vector - insideB.vector).Magnitude() / (outside.vector - insideB.vector).Magnitude();
	beta = 1.0f - alpha;
	newB.vector.w = outside.vector.w * alpha + beta * insideB.vector.w;
	newB.uv.x = insideB.uv.x + alpha * (outside.uv.x - insideB.uv.x);
	newB.uv.y = insideB.uv.y + alpha * (outside.uv.y - insideB.uv.y);
}
void BEngine3D::ClipVertices(Triangle & t, std::vector<Triangle> & output, Plane & plane) {
	int pointsOutside = 0;
	bool vertexOneOutside = false;
	bool vertexTwoOutside = false;
	bool vertexThreeOutside = false;
	BMath::Vec4 dirOne = t.one.vector - plane.pointOnPlane;
	BMath::Vec4 dirTwo = t.two.vector - plane.pointOnPlane;
	BMath::Vec4 dirThree = t.three.vector - plane.pointOnPlane;
	if (dirOne * plane.normal < 0) {
		pointsOutside++;
		vertexOneOutside = true;
	}
	if (dirTwo * plane.normal < 0) {
		pointsOutside++;
		vertexTwoOutside = true;
	}
	if (dirThree * plane.normal < 0) {
		pointsOutside++;
		vertexThreeOutside = true;
	}
	if (pointsOutside == 3) {
		return;
	}
	if (pointsOutside == 2) {
		if (vertexOneOutside && vertexTwoOutside) {
			Vertex newA;
			Vertex newB;
			ClipTwoVertcesOut(t.one, t.two, t.three, newA, newB, plane);
			Triangle newT(newA, newB, t.three);
			newT.color = { 1.0f,1.0f,0.0f };
			output.push_back(newT);
		}
		if (vertexOneOutside && vertexThreeOutside) {
			Vertex newA;
			Vertex newB;
			ClipTwoVertcesOut(t.one, t.three, t.two, newA, newB, plane);
			Triangle newT(newA, t.two, newB);
			newT.color = { 1.0f,1.0f,0.0f };
			output.push_back(newT);
		}
		if (vertexTwoOutside && vertexThreeOutside) {
			Vertex newA;
			Vertex newB;
			ClipTwoVertcesOut(t.two, t.three, t.one, newA, newB, plane);
			Triangle newT(t.one, newA, newB);
			newT.color = { 1.0f,1.0f,0.0f };
			output.push_back(newT);
			DrawString(std::to_string(newA.uv.x), 0, 0, 25, { 1.0f,1.0f,1.0f });
			DrawString(std::to_string(newA.uv.y), 200, 0, 25, { 1.0f,1.0f,1.0f });

			DrawString(std::to_string(newB.uv.x), 0, 25, 25, { 1.0f,1.0f,1.0f });
			DrawString(std::to_string(newB.uv.y), 200, 25, 25, { 1.0f,1.0f,1.0f });

			DrawString(std::to_string(t.two.uv.x), 0, 50, 25, { 1.0f,1.0f,1.0f });
			DrawString(std::to_string(t.two.uv.y), 200, 50, 25, { 1.0f,1.0f,1.0f });

			DrawString(std::to_string(t.three.uv.x), 0, 100, 25, { 1.0f,1.0f,1.0f });
			DrawString(std::to_string(t.three.uv.y), 200, 100, 25, { 1.0f,1.0f,1.0f });

			DrawString(std::to_string(t.one.uv.x), 0, 150, 25, { 1.0f,1.0f,1.0f });
			DrawString(std::to_string(t.one.uv.y), 200, 150, 25, { 1.0f,1.0f,1.0f });

		}
	}
	if (pointsOutside == 1) {
		if (vertexOneOutside) {
			Vertex newA;
			Vertex newB;
			ClipOneVerticeOut(t.one, t.two, t.three, newA, newB, plane);
			Triangle tNew1(newB, newA, t.two);
			Triangle tNew2(newB, t.two, t.three);
			tNew1.color = { 1.0f,0.0f,0.0f };
			tNew2.color = { 0.0f,0.0f,1.0f };
			output.push_back(tNew1);
			output.push_back(tNew2);
		}
		if (vertexTwoOutside) {
			Vertex newA;
			Vertex newB;
			ClipOneVerticeOut(t.two, t.one, t.three, newA, newB, plane);
			Triangle tNew1(t.one, newA, t.three);
			Triangle tNew2(t.three, newA, newB);
			tNew1.color = { 1.0f,0.0f,0.0f };
			tNew2.color = { 0.0f,0.0f,1.0f };
			output.push_back(tNew1);
			output.push_back(tNew2);
		}
		if (vertexThreeOutside) {
			Vertex newA;
			Vertex newB;
			ClipOneVerticeOut(t.three, t.one, t.two, newA, newB, plane);
			Triangle tNew1(t.one, t.two, newB);
			Triangle tNew2(t.one, newB, newA);
			tNew1.color = { 1.0f,0.0f,0.0f };
			tNew2.color = { 0.0f,0.0f,1.0f };
			output.push_back(tNew1);
			output.push_back(tNew2);
			DrawString(std::to_string(newA.uv.x), 0, 0, 25, { 1.0f,1.0f,1.0f });
			DrawString(std::to_string(newA.uv.y), 200, 0, 25, { 1.0f,1.0f,1.0f });

			DrawString(std::to_string(newB.uv.x), 0, 25, 25, { 1.0f,1.0f,1.0f });
			DrawString(std::to_string(newB.uv.y), 200, 25, 25, { 1.0f,1.0f,1.0f });

			DrawString(std::to_string(t.three.uv.x), 0, 50, 25, { 1.0f,1.0f,1.0f });
			DrawString(std::to_string(t.three.uv.y), 200, 50, 25, { 1.0f,1.0f,1.0f });
		}
	}
	if (pointsOutside == 0) {
		output.push_back(t);
	}
}
void BEngine3D::ClipAndDehomogniseVertices(std::vector<Triangle> & triangles, std::vector<Triangle> & output) {
	int screenHeightHalf = GetScreenHeight() / 2;
	int screenWidthHalf = GetScreenWidth() / 2;
	Plane nearPlane({ 0,0,1,0 }, { 0,0,30,1 });
	for (int i = 0; i < triangles.size(); i++) {
		ClipVertices(triangles[i], output, nearPlane);
	}
	for (int i = 0; i < output.size(); i++) {
		Triangle & t = output[i];
		t.one.zInverse = 1 / t.one.vector.w;
		t.two.zInverse = 1 / t.two.vector.w;
		t.three.zInverse = 1 / t.three.vector.w;
		t.one.uv.x /= t.one.vector.w;
		t.one.uv.y /= t.one.vector.w;

		t.two.uv.x /= t.two.vector.w;
		t.two.uv.y /= t.two.vector.w;

		t.three.uv.x /= t.three.vector.w;
		t.three.uv.y /= t.three.vector.w;
		Dehomoginise(output[i].one);
		Dehomoginise(output[i].two);
		Dehomoginise(output[i].three);
		ScaleToViewport(output[i].one, screenWidthHalf, screenHeightHalf);
		ScaleToViewport(output[i].two, screenWidthHalf, screenHeightHalf);
		ScaleToViewport(output[i].three, screenWidthHalf, screenHeightHalf);
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
			float value = 0;
			if (alpha >= value && beta >= value && gamma >= value) {
				BColor finalColor;
				this->shader->FragmentShader(alpha, beta, gamma, one, two, three, finalColor);
				float zBuffer = GetZBuffer(x, y);
				float finalZ = alpha * one.vector.z + beta * two.vector.z + gamma * three.vector.z;
				if (finalZ < zBuffer) {
					SetPixel(x, y, finalColor);
					SetZBuffer(x, y, finalZ);
				}
			}
		}
	}
}

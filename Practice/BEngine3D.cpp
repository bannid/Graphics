#include "BEngine3D.h"


void BEngine3D::DrawMesh(Mesh & mesh) {
	assert(this->initialised);
	if (GetKey('A').keyDown) {
		fov--;
		SetProjectionMatrix();
	}
	if (GetKey('S').keyDown) {
		fov++;
		SetProjectionMatrix();
	}
	DrawString(std::to_string(fov), 500, 0, 25, { 255,255,255 });
	BMath::Mat4 modelMat = mesh.GetModelMat();
	BMath::Vec4 lightDir = { 0,0,-1,0 };
	for (auto it = mesh.triangles.begin(); it != mesh.triangles.end(); it++) {
		Triangle t = *it;
		t.vertices[0].color = { 255,255,255 };
		bool drawTri = true;
		for (int i = 0; i < 3; i++) {
			t.vertices[i].vector = t.vertices[i].vector * modelMat;
			t.vertices[i].normal = t.vertices[i].normal * modelMat;
			if (
				t.vertices[i].vector.z < zNear) {
				drawTri = false;
				break;
			}
			t.vertices[i].vector = t.vertices[i].vector * projectionMatrix;
			t.vertices[i].vector = t.vertices[i].vector * viewPortMatrix;
		}
		if(drawTri)FillTriangleBC(t,&mesh.tex);
		//if (drawTri)DrawTriangle(t);
	}
}

void BEngine3D::Initialise() {
	SetViewportMatrix();
	SetProjectionMatrix();
	SetNDCMatrix();
	this->initialised = true;
}

float Min(float a, float b) {
	return a < b ? a : b;
}
float Max(float a, float b) {
	return a > b ? a : b;
}

void ScaleColor(float t, BColors::color_t & color) {
	if (t < 0)t = 0.05;
	color.red *= t;
	color.green *= t;
	color.blue *= t;
}

void BEngine3D::FillTriangleBC(Triangle & t, Texture * tex) {
	float minX = Min(t.vertices[0].vector.x, Min(t.vertices[1].vector.x,t.vertices[2].vector.x));
	float minY = Min(t.vertices[0].vector.y, Min(t.vertices[1].vector.y, t.vertices[2].vector.y));
	float maxX = Max(t.vertices[0].vector.x, Max(t.vertices[1].vector.x, t.vertices[2].vector.x));
	float maxY = Max(t.vertices[0].vector.y, Max(t.vertices[1].vector.y, t.vertices[2].vector.y));
	//Clamp to screen
	minX = Max(0, minX);
	minY = Max(0, minY);
	maxX = Min(GetScreenWidth(), maxX);
	maxY = Min(GetScreenWidth(), maxY);
	
	BMath::Vec2 pointA = { t.vertices[0].vector.x,t.vertices[0].vector.y };
	BMath::Vec2 pointB = { t.vertices[1].vector.x,t.vertices[1].vector.y };
	BMath::Vec2 pointC = { t.vertices[2].vector.x,t.vertices[2].vector.y };
	BMath::Vec2 edge1 = pointB - pointA;
	BMath::Vec2 edge2 = pointC - pointA;
	BMath::Vec2 edge3 = pointA - pointC;
	BMath::Vec2 edge4 = pointC - pointB;
	
	float mainTriangleArea = (edge1.x * edge2.y - edge1.y * edge2.x) * 0.5f;
	for (int x = minX; x < maxX; x++) {
		for (int y = minY; y < maxY; y++) {
			BMath::Vec2 point = { x,y };

			BMath::Vec2 vp1 = point - pointA;
			float gamma = (edge1.x * vp1.y - edge1.y * vp1.x) * 0.5f;
			gamma /= mainTriangleArea;

			BMath::Vec2 vp2 = point - pointC;
			float beta = (edge3.x * vp2.y - edge3.y * vp2.x) * 0.5;
			beta /= mainTriangleArea;
			
			BMath::Vec2 vp3 = point - pointB;
			float alpha = (edge4.x * vp3.y - edge4.y * vp3.x) * 0.5;
			alpha /= mainTriangleArea;
			
			BMath::Vec4 lightDir = { 0,1,-1,0 };
			lightDir.Normalize();
			t.vertices[0].normal.Normalize();
			t.vertices[1].normal.Normalize();
			t.vertices[2].normal.Normalize();
			float dotAlpha = t.vertices[0].normal * lightDir;
			float dotBeta = t.vertices[1].normal * lightDir;
			float dotGamma = t.vertices[2].normal * lightDir;
			
			float uvXAlpha = t.vertices[0].uv.x;
			float uvYAlpha = t.vertices[0].uv.y;

			float uvXBeta = t.vertices[1].uv.x;
			float uvYBeta = t.vertices[1].uv.y;

			float uvXGamma = t.vertices[2].uv.x;
			float uvYGamma = t.vertices[2].uv.y;

			float finalUVx = alpha * uvXAlpha + beta * uvXBeta + gamma * uvXGamma;
			float finalUVy = alpha * uvYAlpha + beta * uvYBeta + gamma * uvYGamma;
			//We are subtracting from one because model is being flipped.
			BColors::color_t colorFromTex = GetColorFromTexture(finalUVx,1.0f - finalUVy,tex);
			float intensityFinal = dotAlpha * alpha + dotBeta * beta + dotGamma * gamma;
			ScaleColor(intensityFinal, colorFromTex);
			float value = 0;
			if (alpha > value &&
				beta > value &&
				gamma > value) {
					SetPixel(x, y, colorFromTex);
			}
		}
	}
}
void BEngine3D::SetProjectionMatrix() {
	float fovL = DEGREE_TO_RAD(fov);
	float fovInverse = 1 / tanf(fovL/2) * zNear;
	float aspectRatio = (float)GetScreenHeight() / GetScreenWidth();
	projectionMatrix.m[0][0] = aspectRatio * fovInverse;
	projectionMatrix.m[1][1] = fovInverse;
	projectionMatrix.m[2][2] = zFar / (zFar - zNear);
	projectionMatrix.m[3][2] = -zNear * zFar / (zFar-zNear);
	projectionMatrix.m[2][3] = 1;
	projectionMatrix.m[3][3] = 0;
}
void BEngine3D::SetViewportMatrix() {
	float screenHeight = GetScreenHeight();
	float screenWidth = GetScreenWidth();
	float aspectRatio = screenHeight / screenWidth;
	viewPortMatrix.m[0][0] = screenWidth/2.0f;
	viewPortMatrix.m[1][1] = screenHeight/2.0f;
	viewPortMatrix.m[3][0] = screenWidth / 2;
	viewPortMatrix.m[3][1] = screenHeight / 2;
}
void BEngine3D::SetNDCMatrix() {
	NDC.m[0][0] = 1.0f / (screenWidth/2.0f);
	NDC.m[1][1] = 1.0f / (screenHeight/2.0f);
}

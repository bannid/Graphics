#include "BEngine3D.h"
#include "Utils.h"
void BEngine3D::DrawMesh(Mesh & mesh) {
	if (!this->initialised)return;
	BMath::Mat4 modelMat = mesh.GetModelMat();
	BMath::Mat4 viewMatrix = cam.GetViewMatrix().Inverted();
	for (auto it = mesh.triangles.begin(); it != mesh.triangles.end(); it++) {
		Triangle t = *it;
		bool drawTri = true;
		for (int i = 0; i < 3; i++) {
			t.vertices[i].vector = t.vertices[i].vector * modelMat;
			auto mat = modelMat;;
			t.vertices[i].normal = t.vertices[i].normal * mat;
			t.vertices[i].vector = t.vertices[i].vector * viewMatrix;
			if (t.vertices[i].vector.z < zNear) {
				drawTri = false;
				break;
			}
			t.vertices[i].vector = t.vertices[i].vector * projectionMatrix;
			t.vertices[i].vector = t.vertices[i].vector * viewPortMatrix;
		}
		if(drawTri)FillTriangleBC(t,mesh);	
		//if (drawTri)DrawTriangle(t);
	}
}

void BEngine3D::Initialise() {
	SetViewportMatrix();
	SetProjectionMatrix();
	this->initialised = true;
}

void BEngine3D::FillTriangleBC(Triangle & t, Mesh & mesh) {
	float minX = BUtils::Min(t.vertices[0].vector.x, BUtils::Min(t.vertices[1].vector.x,t.vertices[2].vector.x));
	float minY = BUtils::Min(t.vertices[0].vector.y, BUtils::Min(t.vertices[1].vector.y, t.vertices[2].vector.y));
	float maxX = BUtils::Max(t.vertices[0].vector.x, BUtils::Max(t.vertices[1].vector.x, t.vertices[2].vector.x));
	float maxY = BUtils::Max(t.vertices[0].vector.y, BUtils::Max(t.vertices[1].vector.y, t.vertices[2].vector.y));
	//Clamp to screen
	minX = BUtils::Max(0, minX);
	minY = BUtils::Max(0, minY);
	maxX = BUtils::Min(GetScreenWidth(), maxX);
	maxY = BUtils::Min(GetScreenHeight(), maxY);
	
	BMath::Vec2 pointA = { t.vertices[0].vector.x,t.vertices[0].vector.y };
	BMath::Vec2 pointB = { t.vertices[1].vector.x,t.vertices[1].vector.y };
	BMath::Vec2 pointC = { t.vertices[2].vector.x,t.vertices[2].vector.y };
	BMath::Vec2 edge1 = pointB - pointA;
	BMath::Vec2 edge2 = pointC - pointA;
	BMath::Vec2 edge3 = pointA - pointC;
	BMath::Vec2 edge4 = pointC - pointB;
	
	float mainTriangleArea = std::abs((edge1.x * edge2.y - edge1.y * edge2.x) * 0.5f);
	if (mainTriangleArea == 0)return;
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

			float value = 0;
			if (alpha > value && beta > 0 && gamma > 0) {
				BColors::color_t finalColor;
				mesh.shader->GetColor(t, alpha, beta, gamma, mesh, finalColor);
				float zBuffer = GetZBuffer(x, y);
				float finalZ = alpha * t.vertices[0].vector.z + beta * t.vertices[1].vector.z + gamma * t.vertices[2].vector.z;
				if (zBuffer > finalZ) {
					SetPixel(x, y, finalColor);
					SetZBuffer(x, y, finalZ);
				}
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

BMath::Mat4 Camera::GetViewMatrix() {
	BMath::Mat4 mat;
	mat.m[0][0] = this->right.x;
	mat.m[0][1] = this->right.y;
	mat.m[0][2] = this->right.z;
	mat.m[0][3] = this->right.w;

	mat.m[1][0] = this->up.x;
	mat.m[1][1] = this->up.y;
	mat.m[1][2] = this->up.z;
	mat.m[1][3] = this->up.w;

	mat.m[2][0] = this->forward.x;
	mat.m[2][1] = this->forward.y;
	mat.m[2][2] = this->forward.z;
	mat.m[2][3] = this->forward.w;

	mat.m[3][0] = this->position.x;
	mat.m[3][1] = this->position.y;
	mat.m[3][2] = this->position.z;
	mat.m[3][3] = 1;//todo: make sure
	return mat;
}
void Camera::Yaw(float angleInDegrees) {
	BMath::Mat4 mat = BMath::RotationY(angleInDegrees);
	this->right = this->right * mat;
	this->forward = this->forward * mat;
	this->right.Normalize();
	this->forward.Normalize();
	this->up = this->forward.Cross(right).Normalized();
}

void Camera::Pitch(float angleInDegrees) {
	BMath::Mat4 mat = BMath::RotationX(angleInDegrees);
	this->up = this->up * mat;
	this->forward = this->forward * mat;
	this->up.Normalize();
	this->forward.Normalize();
	this->right = this->up.Cross(this->forward).Normalized();
}

void Camera::Roll(float angleInDegrees) {
	BMath::Mat4 mat = BMath::RotationZ(angleInDegrees);
	this->right = this->right * mat;
	this->up = this->up * mat;
	this->right.Normalize();
	this->up.Normalize();
	this->forward = this->up.Cross(this->right).Normalized();
}



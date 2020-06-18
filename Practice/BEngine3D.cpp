#include "BEngine3D.h"
#include "Utils.h"
void BEngine3D::DrawMesh(Mesh & mesh) {
	if (!this->initialised) { 
		return;
	}
	BMath::Mat4 modelMat = mesh.GetModelMat();
	auto m = cam.GetViewMatrix();
	BMath::Mat4 viewMatrix = m.Inverted();
	auto vertices = mesh.vertexes;
	BMath::Mat4 M = modelMat * viewMatrix * projectionMatrix * viewPortMatrix;
	for (auto it = vertices.begin(); it != vertices.end(); it++) {
		auto mat = modelMat;;
		it->normal = it->normal * mat;
		it->vector = it->vector * M;
	}
	for (int i = 0; i < mesh.indices.size(); i += 3) {
		FillTriangleBC(vertices[i], vertices[i + 1], vertices[i + 2], mesh);
	}
}

void BEngine3D::Initialise() {
	SetViewportMatrix();
	SetProjectionMatrix();
	this->initialised = true;
}

void TempGetColor(float alpha, float beta, float gamma,
	Vertex one, Vertex two, Vertex three, Mesh & mesh, BColor & output) {
	//Temp light
	LightDirectional light;
	light.direction = { 0, 0, -1, 0 };
	one.normal.Normalize();
	two.normal.Normalize();
	three.normal.Normalize();
	float dotAlpha = one.normal * light.direction;
	float dotBeta =  two.normal * light.direction;
	float dotGamma = three.normal * light.direction;

	float uvXAlpha = one.uv.x;
	float uvYAlpha = one.uv.y;

	float uvXBeta = two.uv.x;
	float uvYBeta = two.uv.y;

	float uvXGamma = three.uv.x;
	float uvYGamma = three.uv.y;

	float finalUVx = alpha * uvXAlpha + beta * uvXBeta + gamma * uvXGamma;
	float finalUVy = alpha * uvYAlpha + beta * uvYBeta + gamma * uvYGamma;
	output = BUtils::GetColorFromTexture(finalUVx, 1 - finalUVy, &mesh.tex);
	float intensityFinal = dotAlpha * alpha + dotBeta * beta + dotGamma * gamma;
	output.red *= light.color.red;
	output.green *= light.color.green;
	output.blue *= light.color.blue;
	if (intensityFinal < 0.05) {
		intensityFinal = 0.05f;
	}
	output.red *=   intensityFinal;
	output.green *= intensityFinal;
	output.blue *=  intensityFinal;
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

			BMath::Vec2 vp1 = point - pointA;
			float gamma = (edge1.x * vp1.y - edge1.y * vp1.x) * 0.5f;
			gamma /= mainTriangleArea;

			BMath::Vec2 vp2 = point - pointC;
			float beta = (edge3.x * vp2.y - edge3.y * vp2.x) * 0.5;
			beta /= mainTriangleArea;
			BMath::Vec2 vp3 = point - pointB;
			float alpha = (edge4.x * vp3.y - edge4.y * vp3.x) * 0.5;
			alpha /= mainTriangleArea;

			float value = 0 - 0.01;
			if (alpha > value && beta > value && gamma > value) {
				BColor finalColor;
				TempGetColor(alpha, beta, gamma, one, two, three, mesh, finalColor);
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
void BEngine3D::SetProjectionMatrix() {
	float fovL = DEGREE_TO_RAD(fov);
	float fovInverse = 1 / tanf(fovL / 2) * zNear;
	float aspectRatio = (float)GetScreenHeight() / GetScreenWidth();
	projectionMatrix.m[0][0] = aspectRatio * fovInverse;
	projectionMatrix.m[1][1] = fovInverse;
	projectionMatrix.m[2][2] = zFar / (zFar - zNear);
	projectionMatrix.m[3][2] = -zNear * zFar / (zFar - zNear);
	projectionMatrix.m[2][3] = 1;
	projectionMatrix.m[3][3] = 0;
}
void BEngine3D::SetViewportMatrix() {
	int screenHeight = GetScreenHeight();
	int screenWidth = GetScreenWidth();
	viewPortMatrix.m[0][0] = screenWidth / 2.0f;
	viewPortMatrix.m[1][1] = screenHeight / 2.0f;
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
	this->yaw += angleInDegrees * 0.01;
	this->forward.z = std::cos(yaw);
	this->forward.x = std::sin(yaw);
	this->forward.y = std::sin(pitch);
	this->forward.Normalize();
	this->up = { 0.0f,1.0f,0.0f,0.0f };
	this->up.Normalize();
	this->right = this->up.Cross(this->forward);
	this->right.Normalize();
}

void Camera::Pitch(float angleInDegrees) {
	this->pitch += angleInDegrees * 0.01;
	float pitchInDegrees = RAD_TO_DEGREE(pitch);
	if (pitchInDegrees > 89.9)this->pitch = DEGREE_TO_RAD(89.9);
	if (pitchInDegrees < -89.9)this->pitch = DEGREE_TO_RAD(-89.9);
	this->forward.z = std::cos(yaw);
	this->forward.x = std::sin(yaw);
	this->forward.y = std::sin(pitch);
	this->forward.Normalize();
	this->up = { 0.0f,1.0f,0.0f,0.0f };
	this->up.Normalize();
	this->right = this->up.Cross(this->forward);
	this->right.Normalize();
}

void Camera::LookAt(BMath::Vec4 pos, BMath::Vec4 target, BMath::Vec4 up) {
	this->position = pos;
	this->forward = target - pos;
	this->forward.Normalize();
	this->right = up.Cross(forward).Normalized();
	this->up = this->forward.Cross(right).Normalized();
}

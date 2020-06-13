#include "Mesh.h"

Mesh::Mesh(const char * objFile, float size) {
	LoadFromObjFile(objFile);
	this->shader = new GourardShaderTextured();
	this->size = size;
}
Mesh::Mesh(float size) { this->size = size; }
Mesh::Mesh() { this->size = 1; }
bool Mesh::LoadFromObjFile(const char * fileName) {
	std::ifstream f(fileName);
	// Local cache of verts
	std::vector<BMath::Vec4> verts;

	while (!f.eof())
	{
		char line[128];
		f.getline(line, 128);

		std::strstream s;
		s << line;

		char junk;

		if (line[0] == 'v' && line[1] != 't' && line[1] != 'n')
		{
			BMath::Vec4 vertex;
			s >> junk >> vertex.x >> vertex.y >> vertex.z;
			vertex.w = 1;
			this->vertices.push_back(vertex);
		}

		if (line[0] == 'f')
		{
			std::string faces[3];
			s >> junk >> faces[0] >> faces[1] >> faces[2];
			size_t pos = 0;
			std::string delimiter = "/";
			Triangle t;
			for (int i = 0; i < 3; i++) {
				std::string token;
				int index = 0;
				if (faces[i].find(delimiter) != std::string::npos) {
					while ((pos = faces[i].find(delimiter)) != std::string::npos) {
						token = faces[i].substr(0, pos);
						if (token == "") {
							faces[i].erase(0, pos + delimiter.length());
							index++;
							continue;
						}
						int token_ = std::stoi(token);
						if (index == 0) {
							t.vertices[i].vector = this->vertices[token_ - 1];
						}
						else if (index == 1) {
							t.vertices[i].uv = this->textureCoords[token_ - 1];
						}
						faces[i].erase(0, pos + delimiter.length());
						index++;
					}
					token = faces[i];
					int token_ = std::stoi(token);
					t.vertices[i].normal = this->normals[token_ - 1];
				}
				else {
					int index = std::stoi(faces[i]);
					t.vertices[i].vector = this->vertices[index - 1];
				}
			}
			this->triangles.push_back(t);
		}


		if (line[0] == 'v' && line[1] == 't') {
			BMath::Vec4 uv;
			s >> junk >> junk >> uv.x >> uv.y;
			this->textureCoords.push_back(uv);
		}
		if (line[0] == 'v' && line[1] == 'n') {
			BMath::Vec4 normal;
			s >> junk >> junk >> normal.x >> normal.y >> normal.z;
			normal.Normalize();
			this->normals.push_back(normal);
		}
	}
	return true;
}

BMath::Mat4 Mesh::GetModelMat() {
	BMath::Mat4 scale;
	BMath::Mat4 translation;
	BMath::Mat4 orientation;
	orientation.m[0][0] = this->right.x;
	orientation.m[0][1] = this->right.y;
	orientation.m[0][2] = this->right.z;
	orientation.m[0][3] = this->right.w;

	orientation.m[1][0] = this->up.x;
	orientation.m[1][1] = this->up.y;
	orientation.m[1][2] = this->up.z;
	orientation.m[1][3] = this->up.w;

	orientation.m[2][0] = this->forward.x;
	orientation.m[2][1] = this->forward.y;
	orientation.m[2][2] = this->forward.z;
	orientation.m[2][3] = this->forward.w;

	scale.m[0][0] = size;
	scale.m[1][1] = -size;
	scale.m[2][2] = size;

	translation.m[3][0] = position.x;
	translation.m[3][1] = position.y;
	translation.m[3][2] = position.z;
	return orientation * scale * translation;
}
void Mesh::ApplyRotationMatrix(BMath::Mat4 & mat) {
	this->up = this->up * mat;
	this->right = this->right * mat;
	this->forward = this->forward * mat;
}
void Mesh::RotateX(float angleInDegree) {
	BMath::Mat4 mat = BMath::RotationX(angleInDegree);
	this->up = this->up * mat;
	this->right = this->right * mat;
	this->forward = this->forward * mat;
}
void Mesh::RotateY(float angleInDegree){
	BMath::Mat4 mat = BMath::RotationY(angleInDegree);
	this->up = this->up * mat;
	this->right = this->right * mat;
	this->forward = this->forward * mat;
}
void Mesh::RotateZ(float angleInDegree){
	BMath::Mat4 mat = BMath::RotationZ(angleInDegree);
	this->up = this->up * mat;
	this->right = this->right * mat;
	this->forward = this->forward * mat;
}
float Triangle::Area() {
	auto first = this->vertices[1].vector - this->vertices[0].vector;
	auto second = this->vertices[2].vector - this->vertices[0].vector;
	return first.Cross(second).Magnitude() * 0.5;
}

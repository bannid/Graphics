#include "Mesh.h"

Mesh::Mesh(const char * objFile, float size) {
	LoadFromObjFile(objFile);
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
	BMath::Mat4 rotation;
	BMath::Mat4 scale;
	BMath::Mat4 translation;
	rotation.m[0][0] = std::cos(0.01f);
	rotation.m[2][0] = std::sin(0.01f);
	rotation.m[0][2] = std::sin(-0.01f);
	rotation.m[2][2] = std::cos(0.01f);
	up = up * rotation;
	forward = forward * rotation;
	right = right * rotation;
	scale.m[0][0] = size;
	scale.m[1][1] = -size;
	scale.m[2][2] = size;
	rotation.m[0][0] = right.x;
	rotation.m[0][1] = right.y;
	rotation.m[0][2] = right.z;
	rotation.m[0][3] = right.w;

	rotation.m[1][0] = up.x;
	rotation.m[1][1] = up.y;
	rotation.m[1][2] = up.z;
	rotation.m[1][3] = up.w;
	
	rotation.m[2][0] = forward.x;
	rotation.m[2][1] = forward.y;
	rotation.m[2][2] = forward.z;
	rotation.m[2][3] = forward.w;

	translation.m[3][0] = position.x;
	translation.m[3][1] = position.y;
	translation.m[3][2] = position.z;
	return rotation * scale * translation;
}

float Triangle::Area() {
	auto first = this->vertices[1].vector - this->vertices[0].vector;
	auto second = this->vertices[2].vector - this->vertices[0].vector;
	return first.Cross(second).Magnitude() * 0.5;
}
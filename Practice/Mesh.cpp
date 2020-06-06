#include "Mesh.h"
#include "BEngine.h"

Mesh::Mesh(const char * objFile) {
	LoadFromObjFile(objFile);
}

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
	BMath::Mat4 mat;
	mat.m[0][0] = size;
	mat.m[1][1] = -size;
	mat.m[2][2] = size;
	mat.m[3][0] = position.x;
	mat.m[3][1] = position.y;
	mat.m[3][2] = position.z;
	return mat;
}

float Triangle::Area() {
	auto first = this->vertices[1].vector - this->vertices[0].vector;
	auto second = this->vertices[2].vector - this->vertices[0].vector;
	return first.Cross(second).Magnitude() * 0.5;
}
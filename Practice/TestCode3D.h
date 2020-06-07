#pragma once
#include "BEngine3D.h"
#include "Mesh.h"

class TestCode3D : public BEngine3D {
	Mesh triangle;
	Mesh * africanHead;
	virtual bool OnCreate() override {
		triangle = Mesh();
		africanHead = new Mesh("C:\\Users\\Winny-Banni\\Desktop\\videos\\african_head.obj");
		LoadTexturePNG("C:\\Users\\Winny-Banni\\Desktop\\videos\\african_head_diffuse.png",&africanHead->tex,true);
		triangle.vertices.push_back({ -1,0,0,1 });
		triangle.vertices.push_back({ 0,1,0,1 });
		triangle.vertices.push_back({ 1,0,0,1 });
		Triangle t;
		t.vertices[0].vector = triangle.vertices[0];
		t.vertices[1].vector = triangle.vertices[1];
		t.vertices[2].vector = triangle.vertices[2];
		t.vertices[0].normal = { 0,0,-1,0 };
		t.vertices[1].normal = { 0,0,-1,0 };
		t.vertices[2].normal = { 0,0,-1,0 };
		triangle.triangles.push_back(t);
		triangle.position.x = 0;
		triangle.position.y = 0;
		triangle.position.z = 2;
		
		africanHead->position.x = 0;
		africanHead->position.y = 0;
		africanHead->position.z = 1;
		this->Initialise();
		return true;
	}

	virtual bool OnUpdate(float elapsedTime) override {
		ClearScreen(BColors::BLACK);
		//this->DrawMesh(this->triangle);
		this->DrawMesh(*this->africanHead);
		Mesh * selector = this->africanHead;
		if (GetKey(VK_UP).keyDown) {
			selector->position.z += 5.0f * elapsedTime;
		}
		if (GetKey(VK_DOWN).keyDown) {
			selector->position.z -= 5.0f * elapsedTime;
		}
		if (GetKey(VK_RIGHT).keyDown) {
			selector->position.x += 50.0f * elapsedTime;
		}
		if (GetKey(VK_LEFT).keyDown) {
			selector->position.x -= 50.0f * elapsedTime;
		}
		return true;
	}
};
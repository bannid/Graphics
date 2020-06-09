#pragma once
#include "BEngine3D.h"
#include "Mesh.h"

class TestCode3D : public BEngine3D {
	Mesh triangle;
	Mesh * africanHead;
	Mesh * plane;
	virtual bool OnCreate() override {
		triangle = Mesh(1);
		africanHead = new Mesh("C:\\Users\\Winny-Banni\\Desktop\\videos\\african_head.obj",10);
		plane = new Mesh("C:\\Users\\Winny-Banni\\Desktop\\videos\\plane.obj", 10);
		LoadTexturePNG("C:\\Users\\Winny-Banni\\Desktop\\videos\\african_head_diffuse.png",&africanHead->tex,true);
		LoadTexturePNG("C:\\Users\\Winny-Banni\\Pictures\\red.png", &plane->tex, true);
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
		triangle.position.z = 5;
		
		africanHead->position.x = -50;
		africanHead->position.y = 0;
		africanHead->position.z = 50;
		plane->position.x = 0;
		plane->position.y = 0;
		plane->position.z = 500;
		this->Initialise();
		return true;
	}

	virtual bool OnUpdate(float elapsedTime) override {
		ClearScreen(BColors::BLACK);
		ClearZBuffer();
		//this->DrawMesh(*this->plane);
		this->DrawMesh(*this->africanHead);
		
		Mesh * selector = this->africanHead;
		if (GetKey(VK_UP).keyDown) {
			selector->position.z += 50.0f * elapsedTime;
		}
		if (GetKey(VK_DOWN).keyDown) {
			selector->position.z -= 50.0f * elapsedTime;
		}
		if (GetKey(VK_RIGHT).keyDown) {
			selector->position.x += 50.0f * elapsedTime;
		}
		if (GetKey(VK_LEFT).keyDown) {
			selector->position.x -= 50.0f * elapsedTime;
		}
		DrawVector(0, 0, 25, selector->position);
		return true;
	}
};
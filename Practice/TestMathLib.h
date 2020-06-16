#pragma once
#include "Maths.h"
#include "BEngine.h"

class TestMath : public BEngine {
	void PrintAMatrix(BMath::Mat4 & mat) {
		float m11, m12, m13, m14, m21, m22, m23, m24, m31, m32, m33, m34, m41, m42, m43, m44;
		m11 = mat.m[0][0];
		m12 = mat.m[0][1];
		m13 = mat.m[0][2];
		m14 = mat.m[0][3];

		m21 = mat.m[1][0];
		m22 = mat.m[1][1];
		m23 = mat.m[1][2];
		m24 = mat.m[1][3];

		m31 = mat.m[2][0];
		m32 = mat.m[2][1];
		m33 = mat.m[2][2];
		m34 = mat.m[2][3];

		m41 = mat.m[3][0];
		m42 = mat.m[3][1];
		m43 = mat.m[3][2];
		m44 = mat.m[3][3];
		int offsetX = 200;
		int size = 25;
		DrawString(std::to_string(m11), 0, 0, size, 0);
		DrawString(std::to_string(m12), offsetX * 1, 0, size, 0);
		DrawString(std::to_string(m13), offsetX * 2, 0, size, 0);
		DrawString(std::to_string(m14), offsetX * 3, 0, size, 0);

		DrawString(std::to_string(m21), 0, 50, size, 0);
		DrawString(std::to_string(m22), offsetX * 1, 50, size, 0);
		DrawString(std::to_string(m23), offsetX * 2, 50, size, 0);
		DrawString(std::to_string(m24), offsetX * 3, 50, size, 0);

		DrawString(std::to_string(m31), 0, 100, size, 0);
		DrawString(std::to_string(m32), offsetX * 1, 100, size, 0);
		DrawString(std::to_string(m33), offsetX * 2, 100, size, 0);
		DrawString(std::to_string(m34), offsetX * 3, 100, size, 0);

		DrawString(std::to_string(m41), 0, 150, size, 0);
		DrawString(std::to_string(m42), offsetX * 1, 150, size, 0);
		DrawString(std::to_string(m43), offsetX * 2, 150, size, 0);
		DrawString(std::to_string(m44), offsetX * 3, 150, size, 0);
	}
	void PrintAVector(BMath::Vec4 & vec) {
		int offsetX = 200;
		int size = 25;
		DrawString(std::to_string(vec.x), 0, 0, size, 0);
		DrawString(std::to_string(vec.y), offsetX * 1, 0, size, 0);
		DrawString(std::to_string(vec.z), offsetX * 2, 0, size, 0);
		DrawString(std::to_string(vec.w), offsetX * 3, 0, size, 0);
	}
	void MatrixMultiplication() {
		BMath::Mat4 mat = { 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16 };
		BMath::Mat4 matb = { 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16 };
		BMath::Mat4 matTem = mat * matb;
		PrintAMatrix(matTem);
	}
	void MatrixTranspose() {
		BMath::Mat4 mat = { 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16 };
		BMath::Mat4 matb = { 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16 };
		BMath::Mat4 matTem = mat * matb;
		matTem.Transpose();
		matTem.Transpose();
		PrintAMatrix(matTem);
	}
	void MatrixInvert() {
		BMath::Mat4 mat = 
		{5,2,3,9,
		4,7,9,8,
		11,4,3,1,
		19,21,25,31};
		mat.Invert();
		PrintAMatrix(mat);
	}
	void VectorMatMul() {
		BMath::Mat4 mat = { 2,0,0,0,
		0,3,0,0,
		0,0,4,0,
		1,2,3,1 };
		BMath::Vec4 vec = { 2,2,2,1 };
		vec = vec * mat;
		mat.Invert();
		vec = vec * mat;
		PrintAVector(vec);
	}
	virtual bool OnCreate() override {
		return true;
	}

	virtual bool OnUpdate(float elapsedTime) override {
		ClearScreen(WHITE);
		VectorMatMul();
		return true;
	}

};
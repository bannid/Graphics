#pragma once
#include <cmath>
namespace BMath {
	struct Vec2;
	struct Vec4;
	struct Mat4;
	struct Vec2 {
		float x, y;
		Vec2(float x, float y);
		Vec2(int x, int y);
		Vec2();
		float Magnitude();
		void Add(Vec2 that);
		void Subtract(Vec2 & that);
		float Cross(Vec2 that);
		float operator*(Vec2 & that);
		Vec2 operator-(Vec2 & that);
		Vec2 operator+(Vec2 that);
		Vec2 operator*(float scalar);
		void Scale(float scalar);
		void Normalize();
		Vec2 Normalized();
	};
	struct Mat4 {
		float m[4][4];
		Mat4();
		Mat4(float, float, float, float, float, float, float, float, float, float, float, float, float, float, float, float);
		Mat4(int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int);
		void operator=(Mat4 &);
		void Transpose();
		Mat4 Transposed();
		Mat4 Inverted();
		void Invert();
		Mat4 operator*(Mat4 & that);
		void ReduceToZero(int pivotRow, int pivotCol, int targetRow, int targetCol, Mat4&);
	};
	struct Vec4 {
		float x;
		float y;
		float z;
		float w;

		Vec4() :x(0), y(0), z(0), w(0) {}
		Vec4(float x, float y, float z, float w) :x(x), y(y), z(z), w(w) {}
		Vec4(int x, int y, int z, int w) :x(x), y(y), z(z), w(w) {}

		float Magnitude();
		void Subtract(Vec4 & that);
		void Scale(float scalar);
		Vec4 Cross(Vec4 & that);
		Vec4 operator*(float scalar);
		Vec4 operator+(Vec4 & that);
		Vec4 operator-(Vec4 & that);
		float operator*(Vec4 & that);
		Vec4 operator*(Mat4 & mat);
		void operator=(Vec4 that);
		void Normalize();
		Vec4 Normalized();
	};
}
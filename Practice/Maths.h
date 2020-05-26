#pragma once
#include <cmath>
namespace BMath {
	struct Vec2;
	struct Vec4;
	struct Mat4;
	struct Vec2 {
		float x, y;
		Vec2(float x, float y) :x(x), y(y) {}
		Vec2(int x, int y) :x(x), y(y) {}
		Vec2() :x(0), y(0) {}
		float Magnitude() { return std::sqrtf(x * x + y * y); }
		void Add(Vec2 that) {
			this->x += that.x;
			this->y += that.y;
		}
		void Subtract(Vec2 & that) {
			this->x -= that.x;
			this->y -= that.y;
		}
		float Cross(Vec2 that) {
			return this->x * that.y - this->y * that.x;
		}
		float operator*(Vec2 & that) {
			return x * that.x + y * that.y;
		}
		Vec2 operator-(Vec2 & that) {
			return Vec2(x - that.x, y - that.y);
		}
		Vec2 operator+(Vec2 that) {
			return Vec2(x + that.x, y + that.y);
		}
		Vec2 operator*(float scalar) {
			return Vec2(x * scalar, y * scalar);
		}
		void Scale(float scalar) {
			x *= scalar;
			y *= scalar;
		}
		void Normalize() {
			float magnitude = this->Magnitude();
			x /= magnitude;
			y /= magnitude;
		}
		Vec2 Normalized() {
			float magnitude = this->Magnitude();
			return Vec2(x / magnitude, y / magnitude);
		}
	};
	struct Mat4 {
		float m11, m12, m13, m14,
			m21, m22, m23, m24,
			m31, m32, m33, m34,
			m41, m42, m43, m44;
		void Transpose();
		Mat4 Transposed();
		Mat4 Inverted();
		Mat4 Invert();
		Mat4 ReturnIdent();
		void Ident();
		Mat4 operator*(Mat4 that);
		Vec4 operator*(Vec4 that);
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
		void Scale();
		Vec4 Cross(Vec4 that);
		Vec4 operator*(float scalar);
		Vec4 operator+(Vec4 that);
		Vec4 operator-(Vec4 that);
		Vec4 operator*(Vec4 that);
		Vec4 operator*(Mat4 mat);
	};
}
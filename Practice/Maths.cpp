#include <assert.h>
#include "Maths.h"
#include "Common.h"
namespace BMath {
	void Swap(float & a, float & b) {
		float temp = a;
		a = b;
		b = temp;
	}

	Mat4::Mat4() {
		for (int row = 0; row < 4; row++) {
			for (int col = 0; col < 4; col++) {
				if (col == row) {
					m[row][col] = 1;
				}
				else {
					m[row][col] = 0;
				}
			}
		}
	}
	Mat4::Mat4(float m11, float m12, float m13, float m14,
		float m21, float m22, float m23, float m24,
		float m31, float m32, float m33, float m34,
		float m41, float m42, float m43, float m44) {
		this->m[0][0] = m11;
		this->m[0][1] = m12;
		this->m[0][2] = m13;
		this->m[0][3] = m14;

		this->m[1][0] = m21;
		this->m[1][1] = m22;
		this->m[1][2] = m23;
		this->m[1][3] = m24;

		this->m[2][0] = m31;
		this->m[2][1] = m32;
		this->m[2][2] = m33;
		this->m[2][3] = m34;

		this->m[3][0] = m41;
		this->m[3][1] = m42;
		this->m[3][2] = m43;
		this->m[3][3] = m44;

	}

	Mat4::Mat4(int m11, int m12, int m13, int m14,
		int m21, int m22, int m23, int m24,
		int m31, int m32, int m33, int m34,
		int m41, int m42, int m43, int m44) {
		this->m[0][0] = m11;
		this->m[0][1] = m12;
		this->m[0][2] = m13;
		this->m[0][3] = m14;

		this->m[1][0] = m21;
		this->m[1][1] = m22;
		this->m[1][2] = m23;
		this->m[1][3] = m24;

		this->m[2][0] = m31;
		this->m[2][1] = m32;
		this->m[2][2] = m33;
		this->m[2][3] = m34;

		this->m[3][0] = m41;
		this->m[3][1] = m42;
		this->m[3][2] = m43;
		this->m[3][3] = m44;

	}

	void Mat4::operator=(Mat4 that) {
		for (int row = 0; row < 4; row++) {
			for (int col = 0; col < 4; col++) {
				this->m[row][col] = that.m[row][col];
			}
		}
	}
	void Mat4::Transpose() {
		Swap(this->m[0][1], this->m[1][0]);
		Swap(this->m[0][2], this->m[2][0]);
		Swap(this->m[0][3], this->m[3][0]);
		Swap(this->m[1][2], this->m[2][1]);
		Swap(this->m[1][3], this->m[3][1]);
		Swap(this->m[2][3], this->m[3][2]);
	}
	
	Mat4 Mat4::Transposed() {
		Mat4 mat = *this;
		mat.Transpose();
		return mat;
	}

	Mat4 Mat4::operator*(Mat4 & that) {
		Mat4 mat;
		Vec4 row1 = { this->m[0][0],this->m[0][1],this->m[0][2],this->m[0][3] };
		Vec4 row2 = { this->m[1][0],this->m[1][1],this->m[1][2],this->m[1][3] };
		Vec4 row3 = { this->m[2][0],this->m[2][1],this->m[2][2],this->m[2][3] };
		Vec4 row4 = { this->m[3][0],this->m[3][1],this->m[3][2],this->m[3][3] };

		Vec4 col1 = { that.m[0][0],that.m[1][0],that.m[2][0],that.m[3][0] };
		Vec4 col2 = { that.m[0][1],that.m[1][1],that.m[2][1],that.m[3][1] };
		Vec4 col3 = { that.m[0][2],that.m[1][2],that.m[2][2],that.m[3][2] };
		Vec4 col4 = { that.m[0][3],that.m[1][3],that.m[2][3],that.m[3][3] };

		mat.m[0][0] = row1 * col1;
		mat.m[0][1] = row1 * col2;
		mat.m[0][2] = row1 * col3;
		mat.m[0][3] = row1 * col4;

		mat.m[1][0] = row2 * col1;
		mat.m[1][1] = row2 * col2;
		mat.m[1][2] = row2 * col3;
		mat.m[1][3] = row2 * col4;

		mat.m[2][0] = row3 * col1;
		mat.m[2][1] = row3 * col2;
		mat.m[2][2] = row3 * col3;
		mat.m[2][3] = row3 * col4;

		mat.m[3][0] = row4 * col1;
		mat.m[3][1] = row4 * col2;
		mat.m[3][2] = row4 * col3;
		mat.m[3][3] = row4 * col4;
		return mat;
	}
	void Mat4::ReduceToZero(int pivotRow, int pivotCol, int targetRow, int targetCol, Mat4 & mAugmented) {


		if (this->m[targetRow][targetCol] != 0) {

			float a = this->m[targetRow][targetCol];
			float b = this->m[pivotRow][pivotCol];

			this->m[targetRow][0] *= b;
			this->m[targetRow][1] *= b;
			this->m[targetRow][2] *= b;
			this->m[targetRow][3] *= b;

			mAugmented.m[targetRow][0] *= b;
			mAugmented.m[targetRow][1] *= b;
			mAugmented.m[targetRow][2] *= b;
			mAugmented.m[targetRow][3] *= b;

			this->m[targetRow][0] -= this->m[pivotRow][0] * a;
			this->m[targetRow][1] -= this->m[pivotRow][1] * a;
			this->m[targetRow][2] -= this->m[pivotRow][2] * a;
			this->m[targetRow][3] -= this->m[pivotRow][3] * a;

			mAugmented.m[targetRow][0] -= mAugmented.m[pivotRow][0] * a;
			mAugmented.m[targetRow][1] -= mAugmented.m[pivotRow][1] * a;
			mAugmented.m[targetRow][2] -= mAugmented.m[pivotRow][2] * a;
			mAugmented.m[targetRow][3] -= mAugmented.m[pivotRow][3] * a;
			assert(this->m[targetRow][targetCol] == 0);
		}
	}
	void Mat4::Invert() {
		//This funciton will fail if we are doing some 
		//non-uniform scaling. i.e. if we need to swap the rows.
		Mat4 mAugmented;
		ReduceToZero(0, 0, 1, 0, mAugmented);
		ReduceToZero(0, 0, 2, 0, mAugmented);
		ReduceToZero(0, 0, 3, 0, mAugmented);

		ReduceToZero(1, 1, 0, 1, mAugmented);
		ReduceToZero(1, 1, 2, 1, mAugmented);
		ReduceToZero(1, 1, 3, 1, mAugmented);

		ReduceToZero(2, 2, 0, 2, mAugmented);
		ReduceToZero(2, 2, 1, 2, mAugmented);
		ReduceToZero(2, 2, 3, 2, mAugmented);

		ReduceToZero(3, 3, 0, 3, mAugmented);
		ReduceToZero(3, 3, 1, 3, mAugmented);
		ReduceToZero(3, 3, 2, 3, mAugmented);

		for (int i = 0; i < 4; i++) {
			float pivot = this->m[i][i];
			this->m[i][0] /= pivot;
			this->m[i][1] /= pivot;
			this->m[i][2] /= pivot;
			this->m[i][3] /= pivot;

			mAugmented.m[i][0] /= pivot;
			mAugmented.m[i][1] /= pivot;
			mAugmented.m[i][2] /= pivot;
			mAugmented.m[i][3] /= pivot;
		}
		*this = mAugmented;
	}
	Mat4 Mat4::Inverted() {
		Mat4 mat = *this;
		mat.Invert();
		return mat;
	}
	Mat4 RotationZ(float angleInDegrees) {
		float angleInRad = DEGREE_TO_RAD(angleInDegrees);
		Mat4 mat;
		mat.m[0][0] = cos(angleInRad);
		mat.m[0][1] = sin(angleInRad);
		mat.m[1][0] = sin(-angleInRad);
		mat.m[1][1] = cos(angleInRad);
		return mat;
	}
	Mat4 RotationY(float angleInDegrees) {
		float angleInRad = DEGREE_TO_RAD(angleInDegrees);
		Mat4 mat;
		mat.m[0][0] = cos(angleInRad);
		mat.m[0][2] = sin(-angleInRad);
		mat.m[2][0] = sin(angleInRad);
		mat.m[2][2] = cos(angleInRad);
		return mat;
	}
	Mat4 RotationX(float angleInDegrees) {
		float angleInRad = DEGREE_TO_RAD(angleInDegrees);
		Mat4 mat;
		mat.m[1][1] = cos(angleInRad);
		mat.m[1][2] = sin(angleInRad);
		mat.m[2][1] = sin(-angleInRad);
		mat.m[2][2] = cos(angleInRad);
		return mat;
	}
	//############################## VECTORS ####################
	Vec4 Vec4::operator*(float scalar) {
		return Vec4(this->x * scalar, this->y * scalar, this->z * scalar, this->w * scalar);
	}
	float Vec4::operator*(Vec4 & that) {
		return this->x * that.x + this->y * that.y + this->z * that.z + this->w * that.w;
	}
	void Vec4::Scale(float scalar) {
		this->x *= scalar;
		this->y *= scalar;
		this->z *= scalar;
	}
	void Vec4::Subtract(Vec4 & that) {
		this->x -= that.x;
		this->y -= that.y;
		this->z -= that.z;
		this->w -= that.w;
	}
	float Vec4::Magnitude() {
		return std::sqrt(this->x * this->x + this->y * this->y + this->z * this->z);
	}
	Vec4 Vec4::Cross(Vec4 & that) {
		Vec4 vec;
		vec.x = this->y * that.z - this->z * that.y;
		vec.y = this->z * that.x - this->x * that.z;
		vec.z = this->x * that.y - this->y * that.x;
		vec.w = 0;//Cross product returns a normal vector
		return vec;
	}
	Vec4 Vec4::operator+(Vec4 that) {
		Vec4 vec;
		vec.x = this->x + that.x;
		vec.y = this->y + that.y;
		vec.z = this->z + that.z;
		vec.w = this->w + that.w;
		return vec;
	}
	Vec4 Vec4::operator-(Vec4 that) {
		Vec4 vec;
		vec.x = this->x - that.x;
		vec.y = this->y - that.y;
		vec.z = this->z - that.z;
		vec.w = std::abs(this->w - that.w);
		return vec;
	}
	Vec4 Vec4::operator*(Mat4 & mat) {
		Vec4 vec;
		vec.x = this->x * mat.m[0][0] + this->y * mat.m[1][0] + this->z * mat.m[2][0] + this->w * mat.m[3][0];
		vec.y = this->x * mat.m[0][1] + this->y * mat.m[1][1] + this->z * mat.m[2][1] + this->w * mat.m[3][1];
		vec.z = this->x * mat.m[0][2] + this->y * mat.m[1][2] + this->z * mat.m[2][2] + this->w * mat.m[3][2];
		vec.w = this->x * mat.m[0][3] + this->y * mat.m[1][3] + this->z * mat.m[2][3] + this->w * mat.m[3][3];
		if (vec.w != 0.0f) {
			vec.x /= vec.w;
			vec.y /= vec.w;
			vec.z /= vec.w;
			vec.w /= vec.w;
		}
		return vec;
	}
	void Vec4::operator=(Vec4 that) {
		this->x = that.x;
		this->y = that.y;
		this->z = that.z;
		this->w = that.w;
	}
	void Vec4::Normalize() {
		float magn = this->Magnitude();
		this->x /= magn;
		this->y /= magn;
		this->z /= magn;
		this->w /= magn;
	}
	Vec4 Vec4::Normalized() {
		Vec4 vec = *this;
		vec.Normalize();
		return vec;
	}
	//Vectors 2D
	Vec2::Vec2(float x, float y) :x(x), y(y) {}
	Vec2::Vec2(int x, int y) :x(x), y(y) {}
	Vec2::Vec2() :x(0), y(0) {}
	float Vec2::Magnitude() { return std::sqrtf(x * x + y * y); }
	void Vec2::Add(Vec2 that) { this->x += that.x; this->y += that.y; }
	void Vec2::Subtract(Vec2 & that) {
		this->x -= that.x;
		this->y -= that.y;
	}
	float Vec2::Cross(Vec2 that) {
		return this->x * that.y - this->y * that.x;
	}
	float Vec2::operator*(Vec2 & that) {
		return x * that.x + y * that.y;
	}
	Vec2 Vec2::operator-(Vec2 & that) {
		return Vec2(x - that.x, y - that.y);
	}
	Vec2 Vec2::operator+(Vec2 that) {
		return Vec2(x + that.x, y + that.y);
	}
	Vec2 Vec2::operator*(float scalar) {
		return Vec2(x * scalar, y * scalar);
	}
	void Vec2::Scale(float scalar) {
		x *= scalar;
		y *= scalar;
	}
	void Vec2::Normalize() {
		float magnitude = this->Magnitude();
		if (magnitude == 0)return;
		x /= magnitude;
		y /= magnitude;
	}
	Vec2 Vec2::Normalized() {
		float magnitude = this->Magnitude();
		if (magnitude == 0)return Vec2(x, y);
		return Vec2(x / magnitude, y / magnitude);
	}

}
#pragma once
#include "Matrix4x4.h"
#include "Vector3.h"

struct Quaternion {
	float x;
	float y;
	float z;
	float w;
};

Quaternion Add(const Quaternion& q0, const Quaternion& q1);

Quaternion Subtract(const Quaternion& q0, const Quaternion& q1);

Quaternion Multiply(const float& f, const Quaternion& q);

Quaternion Multiply(const Quaternion& lhs, const Quaternion& rhs);

Quaternion IdentityQuaternion();

Quaternion Conjugate(const Quaternion& quaternion);

float Norm(const Quaternion& quaternion);

Quaternion Normalize(const Quaternion& quaternion);

Quaternion Inverse(const Quaternion& quaternion);

//任意軸回転を表すQuaternionの生成
Quaternion MakeRotateAxisAngleQuaternion(const Vector3& axis, float angle);
//ベクトルをQuaternionで回転させた結果のベクトルを求める
Vector3 RotateVector(const Vector3& vector, const Quaternion& quaternion);
//Quaternionから回転行列を求める
Matrix4x4 MakeRotateMatrix(const Quaternion& quaternion);

//内積
float Dot(const Quaternion& q0, const Quaternion& q1);

//球面線形補間
Quaternion Slerp(const Quaternion& q0, const Quaternion& q1, float t);

Quaternion operator+(const Quaternion& q1, const Quaternion& q2);
Quaternion operator-(const Quaternion& q1, const Quaternion& q2);
Quaternion operator*(float s, const Quaternion& q);
Quaternion operator*(const Quaternion& q, float s);
Quaternion operator/(const Quaternion& q, float s);
Quaternion operator-(const Quaternion& q);
Quaternion operator+(const Quaternion& q);
#include "Quaternion.h"
#include "Vector3.h"
#include "Vector3_operation.h"
#include "Matrix4x4_operation.h"
#include <cmath>

Quaternion Add(const Quaternion& q0, const Quaternion& q1) {
	Quaternion returnQuaternion;
	returnQuaternion.x = q0.x + q1.x;
	returnQuaternion.y = q0.y + q1.y;
	returnQuaternion.z = q0.z + q1.z;
	returnQuaternion.w = q0.w + q1.w;
	return returnQuaternion;
}

Quaternion Subtract(const Quaternion& q0, const Quaternion& q1) {
	Quaternion returnQuaternion;
	returnQuaternion.x = q0.x - q1.x;
	returnQuaternion.y = q0.y - q1.y;
	returnQuaternion.z = q0.z - q1.z;
	returnQuaternion.w = q0.w - q1.w;
	return returnQuaternion;
}

Quaternion Multiply(const float& f, const Quaternion& q) {
	Quaternion returnQuaternion;
	returnQuaternion.x = q.x * f;
	returnQuaternion.y = q.y * f;
	returnQuaternion.z = q.z * f;
	returnQuaternion.w = q.w * f;
	return returnQuaternion;
}


Quaternion Multiply(const Quaternion& lhs, const Quaternion& rhs) {
	Quaternion returnQuaternion;
	Vector3 qv = { lhs.x,lhs.y,lhs.z };
	Vector3 rv = { rhs.x,rhs.y,rhs.z };

	returnQuaternion.w = lhs.w * rhs.w - Dot(qv, rv);
	Vector3 ansQ = Cross(qv, rv) + qv * rhs.w + rv * lhs.w;
	returnQuaternion.x = ansQ.x;
	returnQuaternion.y = ansQ.y;
	returnQuaternion.z = ansQ.z;

	return returnQuaternion;
}

Quaternion IdentityQuaternion() {
	Quaternion returnQuaternion{};

	returnQuaternion.w = 1.0f;

	return returnQuaternion;
}

Quaternion Conjugate(const Quaternion& quaternion) {
	Quaternion returnQuaternion{};

	returnQuaternion.x = -quaternion.x;
	returnQuaternion.y = -quaternion.y;
	returnQuaternion.z = -quaternion.z;
	returnQuaternion.w = quaternion.w;

	return returnQuaternion;
}

float Norm(const Quaternion& quaternion) {
	float norm;

	norm = sqrtf(powf(quaternion.w, 2) + powf(quaternion.x, 2) + powf(quaternion.y, 2) + powf(quaternion.z, 2));

	return norm;
}

Quaternion Normalize(const Quaternion& quaternion) {
	Quaternion returnQuaternion;
	float norm = Norm(quaternion);

	returnQuaternion.x = quaternion.x / norm;
	returnQuaternion.y = quaternion.y / norm;
	returnQuaternion.z = quaternion.z / norm;
	returnQuaternion.w = quaternion.w / norm;

	return returnQuaternion;
}

Quaternion Inverse(const Quaternion& quaternion) {
	Quaternion returnQuaternion;
	Quaternion conjugate = Conjugate(quaternion);
	float norm = Norm(quaternion);

	returnQuaternion.x = conjugate.x / powf(norm,2);
	returnQuaternion.y = conjugate.y / powf(norm,2);
	returnQuaternion.z = conjugate.z / powf(norm,2);
	returnQuaternion.w = conjugate.w / powf(norm,2);

	return returnQuaternion;
}

Quaternion MakeRotateAxisAngleQuaternion(const Vector3& axis, float angle) {
	Quaternion returnQuaternion;

	returnQuaternion.w = cosf(angle / 2.0f);
	Vector3 vec = axis * sinf(angle / 2.0f);
	returnQuaternion.x = vec.x;
	returnQuaternion.y = vec.y;
	returnQuaternion.z = vec.z;

	return returnQuaternion;
}

Vector3 RotateVector(const Vector3& vector, const Quaternion& quaternion) {
	Quaternion vecQuaternion;
	vecQuaternion.x = vector.x;
	vecQuaternion.y = vector.y;
	vecQuaternion.z = vector.z;
	vecQuaternion.w = 0.0f;

	vecQuaternion = Multiply(Multiply(quaternion, vecQuaternion), Conjugate(quaternion));

	Vector3 returnVector;
	returnVector.x = vecQuaternion.x;
	returnVector.y = vecQuaternion.y;
	returnVector.z = vecQuaternion.z;

	return returnVector;
}

Matrix4x4 MakeRotateMatrix(const Quaternion& quaternion) {
	Matrix4x4 returnMatrix;

	// w^2 - x^2 - y^2 ^ z^2
	Matrix4x4 S = MakeScaleMatrix(Vector3{ 
		powf(quaternion.w,2) - powf(quaternion.x,2) - powf(quaternion.y,2) - powf(quaternion.z,2),
		powf(quaternion.w,2) - powf(quaternion.x,2) - powf(quaternion.y,2) - powf(quaternion.z,2) ,
		powf(quaternion.w,2) - powf(quaternion.x,2) - powf(quaternion.y,2) - powf(quaternion.z,2)
		});
	// -wx,-wy,-wz
	Matrix4x4 C = CrossMatrix(Vector3{
		-(quaternion.w * quaternion.x),
		-(quaternion.w * quaternion.y),
		-(quaternion.w * quaternion.z)
		});
	// x,y,z * x,y,z
	Matrix4x4 D = DotMatrix(
		Vector3{ quaternion.x,quaternion.y,quaternion.z },
		Vector3{ quaternion.x,quaternion.y,quaternion.z }
	);

	returnMatrix = S + 2.0f * (D + C);
	returnMatrix.m[3][3] = 1.0f;
	return returnMatrix;
}

float Dot(const Quaternion& q0, const Quaternion& q1) {
	return q0.x * q1.x + q0.y * q1.y + q0.z * q1.z + q0.w * q1.w;
}

Quaternion Slerp(const Quaternion& q0, const Quaternion& q1, float t) {
	Quaternion Q0 = q0, Q1 = q1;
	float dot = Dot(Q0, Q1);
	if (dot < 0) {
		Q0 = -Q0;
		dot = -dot;
	}

	float theta = std::acosf(dot);

	float scale0 = std::sinf((1 - t) * theta) / std::sinf(theta);
	float scale1 = std::sinf(t * theta) / std::sinf(theta);

	return scale0 * Q0 + scale1 * Q1;
}

Quaternion operator+(const Quaternion& q1, const Quaternion& q2) { return Add(q1, q2); }
Quaternion operator-(const Quaternion& q1, const Quaternion& q2) { return Subtract(q1, q2); }
Quaternion operator*(float s, const Quaternion& q) { return Multiply(s, q); }
Quaternion operator*(const Quaternion& q, float s) { return s * q; }
Quaternion operator/(const Quaternion& q, float s) { return (1 / s) * q; }
Quaternion operator-(const Quaternion& q) { return Quaternion{ -q.x,-q.y,-q.z,-q.w }; }
Quaternion operator+(const Quaternion& q) { return q; }
#include "Quaternion.h"
#include "Vector3.h"
#include "Vector3_operation.h"
#include <cmath>

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
	returnQuaternion.y= conjugate.y / powf(norm,2);
	returnQuaternion.z = conjugate.z / powf(norm,2);
	returnQuaternion.w = conjugate.w / powf(norm,2);

	return returnQuaternion;
}
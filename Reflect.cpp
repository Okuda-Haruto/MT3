#include "Reflect.h"
#include "Vector3_operation.h"

Vector3 Reflect(const Vector3& input, const Vector3& normal) {
	Vector3 r;
	r = input - (Dot(input, normal) * 2) * normal;
	return r;
}
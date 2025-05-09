#include "Collision.h"
#include "Vector3_operation.h"
#include <cmath>

//正射影ベクトル
Vector3 Project(const Vector3& v1, const Vector3& v2) {
	float t;
	Vector3 vector;
	t = Dot(v1,v2) / powf(Length(v2),2);
	vector = Multiply(t, v2);
	return vector;
}
//最終接点
Vector3 ClosestPoint(const Vector3& point, const Segment& segment) {
	Vector3 cp;
	Vector3 a = Subtract(Subtract(point, segment.origin), segment.diff);
	Vector3 b = Subtract(point, segment.origin);
	float t;
	t = Dot(a, b) / powf(Length(b), 2);
	cp = Add(segment.origin, Project(b, segment.diff));
	return cp;
}
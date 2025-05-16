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
	Vector3 a = Subtract(point, segment.origin);
	cp = Add(segment.origin, Project(a, segment.diff));
	return cp;
}

//球と球の衝突
bool IsCollision(const Sphere& s1, const Sphere& s2) {
	float distance = Length(Subtract(s2.center,s1.center));

	return distance <= s1.radius + s2.radius;
}

//球と平面の衝突
bool IsCollision(const Sphere& sphere, const Plane& plane) {
	float k = Dot(plane.normal, sphere.center) - plane.distance;

	return fabsf(k) <= sphere.radius;
}
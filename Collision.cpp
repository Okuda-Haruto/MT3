#include "Collision.h"
#include "Vector3_operation.h"
#include "Matrix4x4_operation.h"
#include <cmath>
#include <algorithm>

Vector3 ClosestPointOnTriangle(const Vector3& p, const Triangle& tri) {
	// 頂点
	const Vector3& a = tri.vertices[0];
	const Vector3& b = tri.vertices[1];
	const Vector3& c = tri.vertices[2];

	// 辺
	Vector3 ab = Subtract(b, a);
	Vector3 ac = Subtract(c, a);

	Vector3 ap = Subtract(p, a);
	float d1 = Dot(ab, ap);
	float d2 = Dot(ac, ap);
	if (d1 <= 0 && d2 <= 0) return a;  // vertex A

	Vector3 bp = Subtract(p, b);
	float d3 = Dot(ab, bp);
	float d4 = Dot(ac, bp);
	if (d3 >= 0 && d4 <= d3) return b;  // vertex B

	Vector3 cp = Subtract(p, c);
	float d5 = Dot(ab, cp);
	float d6 = Dot(ac, cp);
	if (d6 >= 0 && d5 <= d6) return c;  // vertex C

	float vc = d1 * d4 - d3 * d2;
	if (vc <= 0 && d1 >= 0 && d3 <= 0) {
		float v = d1 / (d1 - d3);
		return Add(a,Multiply(v,ab));  // edge AB
	}

	float vb = d5 * d2 - d1 * d6;
	if (vb <= 0 && d2 >= 0 && d6 <= 0) {
		float w = d2 / (d2 - d6);
		return Add(a, Multiply(w, ac));  // edge AC
	}

	float va = d3 * d6 - d5 * d4;
	if (va <= 0 && (d4 - d3) >= 0 && (d5 - d6) >= 0) {
		Vector3 bc = Subtract(c, b);
		float w = (d4 - d3) / ((d4 - d3) + (d5 - d6));
		return Add(b, Multiply(w, bc));  // edge BC
	}

	// 面内部
	float denom = 1 / (va + vb + vc);
	float v = vb * denom;
	float w = vc * denom;
	return Add(a, Add(Multiply(v, ab), Multiply(w, ac)));
}

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

//球と3角形の衝突
bool IsCollision(const Sphere& sphere, const Triangle& triangle) {
	Vector3 v01 = Subtract(triangle.vertices[0], triangle.vertices[1]);
	Vector3 v12 = Subtract(triangle.vertices[1], triangle.vertices[2]);
	Vector3 v20 = Subtract(triangle.vertices[2], triangle.vertices[0]);
	Plane plane;
	plane.normal = Normalize(Cross(v01, v12));
	plane.distance = Dot(triangle.vertices[0], plane.normal);

	if (IsCollision(sphere, plane)) {
		//衝突地点の計算
		Vector3 closest = ClosestPointOnTriangle(sphere.center, triangle);	//最近接点
		Vector3 diff = Subtract(sphere.center,closest);
		return Dot(diff, diff) <= sphere.radius * sphere.radius;
	}

	return false;
}

//直線と平面の衝突
bool IsCollision(const Line& line, const Plane& plane) {
	if ((plane.distance - Dot(line.origin, plane.normal)) == 0.0f) {
		return false;
	}

	return true;
}

//半直線と平面の衝突
bool IsCollision(const Ray& ray, const Plane& plane) {
	if ((plane.distance - Dot(ray.origin, plane.normal)) == 0.0f) {
		return false;
	}

	float t = (plane.distance - Dot(ray.origin, plane.normal)) / Dot(ray.diff, plane.normal);

	if (t > 0.0f) {
		return true;
	} else {
		return false;
	}
}

//線分と平面の衝突
bool IsCollision(const Segment& segment, const Plane& plane) {
	if ((plane.distance - Dot(segment.origin, plane.normal)) == 0.0f) {
		return false;
	}

	float t = (plane.distance - Dot(segment.origin, plane.normal)) / Dot(segment.diff, plane.normal);

	if (t > 0.0f && t <= 1.0f) {
		return true;
	} else {
		return false;
	}
}

//三角形と線分の衝突
bool IsCollision(const Triangle& triangle, const Segment& segment) {
	Vector3 v01 = Subtract(triangle.vertices[0], triangle.vertices[1]);
	Vector3 v12 = Subtract(triangle.vertices[1], triangle.vertices[2]);
	Vector3 v20 = Subtract(triangle.vertices[2], triangle.vertices[0]);
	Plane plane;
	plane.normal = Normalize(Cross(v01, v12));
	plane.distance = Dot(triangle.vertices[0], plane.normal);

	if (IsCollision(segment, plane)) {
		float t = (plane.distance - Dot(segment.origin, plane.normal)) / Dot(segment.diff, plane.normal);
		Vector3 p = Add(segment.origin, Multiply(t, segment.diff));
		Vector3 v0p = Subtract(triangle.vertices[0], p);
		Vector3 v1p = Subtract(triangle.vertices[1], p);
		Vector3 v2p = Subtract(triangle.vertices[2], p);

		Vector3 cross01 = Cross(v01, v1p);
		Vector3 cross12 = Cross(v12, v2p);
		Vector3 cross20 = Cross(v20, v0p);

		if (Dot(cross01, plane.normal) >= 0.0f &&
			Dot(cross12, plane.normal) >= 0.0f &&
			Dot(cross20, plane.normal) >= 0.0f) {
			return true;
		}
	}

	return false;
}

//AABBとAABBの衝突
bool IsCollision(const AABB& aabb1, const AABB& aabb2) {
	if ((aabb1.min.x <= aabb2.max.x && aabb2.min.x <= aabb1.max.x) &&
		(aabb1.min.y <= aabb2.max.y && aabb2.min.y <= aabb1.max.y) &&
		(aabb1.min.z <= aabb2.max.z && aabb2.min.z <= aabb1.max.z)) {
		return true;
	}
	return false;
}

//AABBと球の衝突
bool IsCollision(const AABB& aabb, const Sphere& sphere) {
	Vector3 clossPoint{
		std::clamp(sphere.center.x,aabb.min.x,aabb.max.x),
		std::clamp(sphere.center.y,aabb.min.y,aabb.max.y),
		std::clamp(sphere.center.z,aabb.min.z,aabb.max.z)
	};

	//最近接点と球の中心との距離を求める
	float distance = Length(Subtract(clossPoint, sphere.center));

	//距離が半径よりも小さければ衝突
	if (distance <= sphere.radius) {
		return true;
	}
	return false;
}

//AABBと直線の衝突
bool IsCollision(const AABB& aabb, const Line& line) {

	Vector3 min = {
		(aabb.min.x - line.origin.x) / line.diff.x,
		(aabb.min.y - line.origin.y) / line.diff.y,
		(aabb.min.z - line.origin.z) / line.diff.z,
	};
	Vector3 max = {
		(aabb.max.x - line.origin.x) / line.diff.x,
		(aabb.max.y - line.origin.y) / line.diff.y,
		(aabb.max.z - line.origin.z) / line.diff.z,
	};

	float tNearX = std::min(min.x, max.x), tFarX = std::max(min.x, max.x);
	float tNearY = std::min(min.y, max.y), tFarY = std::max(min.y, max.y);
	float tNearZ = std::min(min.z, max.z), tFarZ = std::max(min.z, max.z);

	//AABBとの衝突点(貫通点)のtが小さい方
	float tmin = std::max(std::max(tNearX, tNearY), tNearZ);
	//AABBとの衝突点(貫通点)のtが大きい方
	float tmax = std::min(std::min(tFarX, tFarY), tFarZ);

	if (tmin <= tmax) {
		return true;
	}
	return false;
}

//AABBと半直線の衝突
bool IsCollision(const AABB& aabb, const Ray& ray) {

	Vector3 min = {
		(aabb.min.x - ray.origin.x) / ray.diff.x,
		(aabb.min.y - ray.origin.y) / ray.diff.y,
		(aabb.min.z - ray.origin.z) / ray.diff.z,
	};
	Vector3 max = {
		(aabb.max.x - ray.origin.x) / ray.diff.x,
		(aabb.max.y - ray.origin.y) / ray.diff.y,
		(aabb.max.z - ray.origin.z) / ray.diff.z,
	};

	float tNearX = std::min(min.x, max.x), tFarX = std::max(min.x, max.x);
	float tNearY = std::min(min.y, max.y), tFarY = std::max(min.y, max.y);
	float tNearZ = std::min(min.z, max.z), tFarZ = std::max(min.z, max.z);

	//AABBとの衝突点(貫通点)のtが小さい方
	float tmin = std::max(std::max(tNearX, tNearY), tNearZ);
	//AABBとの衝突点(貫通点)のtが大きい方
	float tmax = std::min(std::min(tFarX, tFarY), tFarZ);

	if (tmin <= tmax && tmax >= 0.0f) {
		return true;
	}
	return false;
}

//AABBと線分の衝突
bool IsCollision(const AABB& aabb, const Segment& segment) {

	Vector3 min = {
		(aabb.min.x - segment.origin.x) / segment.diff.x,
		(aabb.min.y - segment.origin.y) / segment.diff.y,
		(aabb.min.z - segment.origin.z) / segment.diff.z,
	};
	//Nan対策
	if ((aabb.min.x - segment.origin.x) == 0 && segment.diff.x == 0) {
		min.x = (aabb.min.x - (segment.origin.x - 0.00001f)) / (segment.diff.x - 0.00001f);
	}
	if ((aabb.min.y - segment.origin.y) == 0 && segment.diff.y == 0) {
		min.y = (aabb.min.y - (segment.origin.y - 0.00001f)) / (segment.diff.y - 0.00001f);
	}
	if ((aabb.min.z - segment.origin.z) == 0 && segment.diff.z == 0) {
		min.z = (aabb.min.z - (segment.origin.z - 0.00001f)) / (segment.diff.z - 0.00001f);
	}

	Vector3 max = {
		(aabb.max.x - segment.origin.x) / segment.diff.x,
		(aabb.max.y - segment.origin.y) / segment.diff.y,
		(aabb.max.z - segment.origin.z) / segment.diff.z,
	};
	//Nan対策
	if ((aabb.max.x - segment.origin.x) == 0 && segment.diff.x == 0) {
		max.x = (aabb.max.x - (segment.origin.x - 0.00001f)) / (segment.diff.x - 0.00001f);
	}
	if ((aabb.max.y - segment.origin.y) == 0 && segment.diff.y == 0) {
		max.y = (aabb.max.y - (segment.origin.y - 0.00001f)) / (segment.diff.y - 0.00001f);
	}
	if ((aabb.max.z - segment.origin.z) == 0 && segment.diff.z == 0) {
		max.z = (aabb.max.z - (segment.origin.z - 0.00001f)) / (segment.diff.z - 0.00001f);
	}

	float tNearX = std::min(min.x, max.x), tFarX = std::max(min.x, max.x);
	float tNearY = std::min(min.y, max.y), tFarY = std::max(min.y, max.y);
	float tNearZ = std::min(min.z, max.z), tFarZ = std::max(min.z, max.z);

	//AABBとの衝突点(貫通点)のtが小さい方
	float tmin = std::max(std::max(tNearX, tNearY), tNearZ);
	//AABBとの衝突点(貫通点)のtが大きい方
	float tmax = std::min(std::min(tFarX, tFarY), tFarZ);

	if (fabsf(tmin) == INFINITY || fabsf(tmax) == INFINITY) {
		return true;
	}

	if (tmin <= tmax && tmin <= 1.0f && tmax >= 0.0f) {
		return true;
	}
	return false;
}

//OBBと球の衝突
bool IsCollision(const OBB& obb, const Sphere& sphere) {

	Matrix4x4 obbWorldMatrix{
		.m{
			{obb.orientations[0].x	,obb.orientations[0].y	,obb.orientations[0].z	,0.0f},
			{obb.orientations[1].x	,obb.orientations[1].y	,obb.orientations[1].z	,0.0f},
			{obb.orientations[2].x	,obb.orientations[2].y	,obb.orientations[2].z	,0.0f},
			{obb.center.x			,obb.center.y			,obb.center.z			,1.0f},
		}
	};

	Matrix4x4 obbWorldMatrixInverse = Inverse(obbWorldMatrix);

	Vector3 centerInOBBLocalSpace = Transform(sphere.center, obbWorldMatrixInverse);

	AABB aabbOBBLocal{
		.min{-obb.size.x,-obb.size.y,-obb.size.z},
		.max{obb.size}
	};

	Sphere sphereOBBLocal{
		.center{centerInOBBLocalSpace},
		.radius{sphere.radius}
	};

	return IsCollision(aabbOBBLocal, sphereOBBLocal);
}

//OBBと直線の衝突
bool IsCollision(const OBB& obb, const Line& line) {

	Matrix4x4 obbWorldMatrix{
		.m{
			{obb.orientations[0].x	,obb.orientations[0].y	,obb.orientations[0].z	,0.0f},
			{obb.orientations[1].x	,obb.orientations[1].y	,obb.orientations[1].z	,0.0f},
			{obb.orientations[2].x	,obb.orientations[2].y	,obb.orientations[2].z	,0.0f},
			{obb.center.x			,obb.center.y			,obb.center.z			,1.0f},
		}
	};

	Matrix4x4 obbWorldMatrixInverse = Inverse(obbWorldMatrix);

	Vector3 localOrigin = Transform(line.origin, obbWorldMatrixInverse);
	Vector3 localEnd = Transform(Add(line.origin, line.diff), obbWorldMatrixInverse);

	AABB localAABB{
		{-obb.size.x,-obb.size.y,-obb.size.z},
		{+obb.size.x,+obb.size.y,+obb.size.z}
	};

	Line localLine;
	localLine.origin = localOrigin;
	localLine.diff = Subtract(localEnd, localOrigin);

	return IsCollision(localAABB, localLine);
}

//OBBと半直線の衝突
bool IsCollision(const OBB& obb, const Ray& ray) {

	Matrix4x4 obbWorldMatrix{
		.m{
			{obb.orientations[0].x	,obb.orientations[0].y	,obb.orientations[0].z	,0.0f},
			{obb.orientations[1].x	,obb.orientations[1].y	,obb.orientations[1].z	,0.0f},
			{obb.orientations[2].x	,obb.orientations[2].y	,obb.orientations[2].z	,0.0f},
			{obb.center.x			,obb.center.y			,obb.center.z			,1.0f},
		}
	};

	Matrix4x4 obbWorldMatrixInverse = Inverse(obbWorldMatrix);

	Vector3 localOrigin = Transform(ray.origin, obbWorldMatrixInverse);
	Vector3 localEnd = Transform(Add(ray.origin, ray.diff), obbWorldMatrixInverse);

	AABB localAABB{
		{-obb.size.x,-obb.size.y,-obb.size.z},
		{+obb.size.x,+obb.size.y,+obb.size.z}
	};

	Ray localRay;
	localRay.origin = localOrigin;
	localRay.diff = Subtract(localEnd, localOrigin);

	return IsCollision(localAABB, localRay);
}

//OBBと線分の衝突
bool IsCollision(const OBB& obb, const Segment& segment) {

	Matrix4x4 obbWorldMatrix{
		.m{
			{obb.orientations[0].x	,obb.orientations[0].y	,obb.orientations[0].z	,0.0f},
			{obb.orientations[1].x	,obb.orientations[1].y	,obb.orientations[1].z	,0.0f},
			{obb.orientations[2].x	,obb.orientations[2].y	,obb.orientations[2].z	,0.0f},
			{obb.center.x			,obb.center.y			,obb.center.z			,1.0f},
		}
	};

	Matrix4x4 obbWorldMatrixInverse = Inverse(obbWorldMatrix);

	Vector3 localOrigin = Transform(segment.origin, obbWorldMatrixInverse);
	Vector3 localEnd = Transform(Add(segment.origin, segment.diff), obbWorldMatrixInverse);

	AABB localAABB{
		{-obb.size.x,-obb.size.y,-obb.size.z},
		{+obb.size.x,+obb.size.y,+obb.size.z}
	};

	Segment localSegment;
	localSegment.origin = localOrigin;
	localSegment.diff = Subtract(localEnd, localOrigin);

	return IsCollision(localAABB, localSegment);
}

//OBBとOBBの衝突
/*bool IsCollision(const OBB& obb1, const OBB& obb2) {

	Matrix4x4 obb1WorldMatrix{
		.m{
			{obb1.orientations[0].x	,obb1.orientations[0].y	,obb1.orientations[0].z	,0.0f},
			{obb1.orientations[1].x	,obb1.orientations[1].y	,obb1.orientations[1].z	,0.0f},
			{obb1.orientations[2].x	,obb1.orientations[2].y	,obb1.orientations[2].z	,0.0f},
			{obb1.center.x			,obb1.center.y			,obb1.center.z			,1.0f},
		}
	};
	Matrix4x4 obb1WorldMatrixInverse = Inverse(obb1WorldMatrix);

	Matrix4x4 obb2WorldMatrix{
		.m{
			{obb2.orientations[0].x	,obb2.orientations[0].y	,obb2.orientations[0].z	,0.0f},
			{obb2.orientations[1].x	,obb2.orientations[1].y	,obb2.orientations[1].z	,0.0f},
			{obb2.orientations[2].x	,obb2.orientations[2].y	,obb2.orientations[2].z	,0.0f},
			{obb2.center.x			,obb2.center.y			,obb2.center.z			,1.0f},
		}
	};
	Matrix4x4 obb2WorldMatrixInverse = Inverse(obb2WorldMatrix);

	return false;
}*/

//分離軸
bool SAT(const AABB& aabb, const Triangle& triangle, Vector3 axis) {
	//3角形の投射
	float p0 = Dot(triangle.vertices[0], axis);
	float p1 = Dot(triangle.vertices[1], axis);
	float p2 = Dot(triangle.vertices[2], axis);
	float triangleMin = std::min(std::min(p0, p1), p2);
	float triangleMax = std::max(std::max(p0, p1), p2);
	//AABBの投射
	float aabbMin = std::min(Dot(aabb.min, axis), Dot(aabb.max, axis));
	float aabbMax = std::max(Dot(aabb.max, axis), Dot(aabb.min, axis));
	//影の長さ
	float L1 = triangleMax - triangleMin;
	float L2 = aabbMax - aabbMin;
	float sumSpan = L1 + L2;
	//2つの影の両端の差分
	float longSpan = std::max(triangleMax, aabbMax) - std::min(triangleMin, aabbMin);

	if (sumSpan < longSpan) {
		return false;
	}

	return true;
}

//AABBと3角形の衝突
bool IsCollision(const AABB& aabb, const Triangle& triangle) {

	//軸に平行な法線
	Vector3 axis[3]{
		{1.0f,0.0f,0.0f},
		{0.0f,1.0f,0.0f},
		{0.0f,0.0f,1.0f},
	};
	//3角形の辺
	Vector3 side[3]{
		Subtract(triangle.vertices[1], triangle.vertices[0]),
		Subtract(triangle.vertices[2], triangle.vertices[1]),
		Subtract(triangle.vertices[0], triangle.vertices[2]),
	};
	//3角形の法線
	Vector3 n = Normalize(Cross(side[0], side[1]));

	//3角形の法線
	if (!SAT(aabb, triangle, n)) {
		return false;
	}

	//X~Z
	for (int i = 0; i < 3; i++) {
		if (!SAT(aabb, triangle, axis[i])) {
			return false;
		}
	}

	Vector3 axisN[3]{
		{n.x,0.0f,0.0f},
		{0.0f,n.y,0.0f},
		{0.0f,0.0f,n.z}
	};

	//3角形の辺と軸による9つのクロス積
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			Vector3 crossAxis = Normalize(Cross(axis[i], axisN[j]));
			if (!SAT(aabb, triangle, crossAxis)) {
				return false;
			}
		}
	}

	return true;
}

//カプセルと平面の衝突
bool IsCollision(const Capsule& capsule, const Plane& plane) {
	//線分が接触しているなら接触している
	if (IsCollision(capsule.segment, plane)) { return true; }
	
	//線分の始点か終点の球のどちらかが接触しているなら接触している
	if (IsCollision(Sphere{ capsule.segment.origin,capsule.radius }, plane)) { return true; }
	if (IsCollision(Sphere{ capsule.segment.origin + capsule.segment.diff,capsule.radius }, plane)) { return true; }

	//そうでないなら接触していない
	return false;
}
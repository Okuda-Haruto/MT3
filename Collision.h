#pragma once
#include "Line.h"
#include "Sphere.h"
#include "Plane.h"
#include "Triangle.h"
#include "AABB.h"

//正射影ベクトル
Vector3 Project(const Vector3& v1, const Vector3& v2);
//最終接点
Vector3 ClosestPoint(const Vector3& point, const Segment& segment);

//球と球の衝突
bool IsCollision(const Sphere& s1, const Sphere& s2);
//球と平面の衝突
bool IsCollision(const Sphere& sphere, const Plane& plane);
//直線と平面の衝突
bool IsCollision(const Line& line, const Plane& plane);
//半直線と平面の衝突
bool IsCollision(const Ray& ray, const Plane& plane);
//線分と平面の衝突
bool IsCollision(const Segment& segment, const Plane& plane);
//三角形と線分の衝突
bool IsCollision(const Triangle& triangle, const Segment& segment);
//AABBとAABBの衝突
bool IsCollision(const AABB& aabb1, const AABB& aabb2);
//AABBと球の衝突
bool IsCollision(const AABB& aabb, const Sphere& sphere);
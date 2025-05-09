#pragma once
#include "Line.h"
#include "Sphere.h"

//正射影ベクトル
Vector3 Project(const Vector3& v1, const Vector3& v2);
//最終接点
Vector3 ClosestPoint(const Vector3& point, const Segment& segment);

//球と球の衝突
bool IsCollision(const Sphere& s1, const Sphere& s2);
#pragma once
#include "Line.h"

//正射影ベクトル
Vector3 Project(const Vector3& v1, const Vector3& v2);
//最終接点
Vector3 ClosestPoint(const Vector3& point, const Segment& segment);
#pragma once
#include "Vector3.h"
#include "Matrix4x4.h"
#include <format>

void DrawBezier(
	const Vector3& controlPoint0,
	const Vector3& controlPoint1,
	const Vector3& controlPoint2,
	const Matrix4x4& viewProjectionMatrix,
	const Matrix4x4& viewportMatrix,
	uint32_t color);
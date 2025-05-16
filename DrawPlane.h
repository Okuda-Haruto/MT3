#pragma once
#include "Plane.h"
#include "Matrix4x4.h"
#include <format>

void DrawPlane(const Plane& plane, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color);
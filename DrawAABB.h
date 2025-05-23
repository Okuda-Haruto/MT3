#pragma once
#include "Matrix4x4.h"
#include "AABB.h"
#include <format>

void DrawAABB(const AABB& aabb, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color);
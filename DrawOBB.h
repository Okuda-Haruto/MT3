#pragma once
#include "Matrix4x4.h"
#include "OBB.h"
#include <format>

void DrawOBB(const OBB& obb, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color);
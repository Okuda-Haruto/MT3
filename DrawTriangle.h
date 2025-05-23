#pragma once
#include "Triangle.h"
#include "Matrix4x4.h"
#include <format>

void DrawTriangle(const Triangle& triangle, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color);
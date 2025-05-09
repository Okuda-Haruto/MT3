#pragma once
#include "Sphere.h"
#include <Matrix4x4.h>
#define _USE_MATH_DEFINES
#include <cmath>
#include <format>

void DrawSphere(const Sphere& sphere, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color);
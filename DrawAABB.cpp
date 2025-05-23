#include "DrawAABB.h"
#include "Matrix4x4_operation.h"
#include <Novice.h>

void DrawAABB(const AABB& aabb, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color) {
	Vector3 vertices[8];
	Vector3 points[8];

	for (int i = 0; i < 8; i++) {
		//	X
		if (i < 4) {
			vertices[i].x = aabb.min.x;
		} else {
			vertices[i].x = aabb.max.x;
		}
		//	Y
		if ((i / 2) % 2 == 0) {
			vertices[i].y = aabb.min.y;
		} else {
			vertices[i].y = aabb.max.y;
		}
		//	Z
		if (i % 2 == 0) {
			vertices[i].z = aabb.min.z;
		} else {
			vertices[i].z = aabb.max.z;
		}

		points[i] = Transform(Transform(vertices[i], viewProjectionMatrix), viewportMatrix);
	}

	for (int i = 0; i < 2; i++) {
		Novice::DrawLine(int(points[i * 4 + 0].x), int(points[i * 4 + 0].y), int(points[i * 4 + 1].x), int(points[i * 4 + 1].y), color);
		Novice::DrawLine(int(points[i * 4 + 2].x), int(points[i * 4 + 2].y), int(points[i * 4 + 3].x), int(points[i * 4 + 3].y), color);
		Novice::DrawLine(int(points[i * 4 + 0].x), int(points[i * 4 + 0].y), int(points[i * 4 + 2].x), int(points[i * 4 + 2].y), color);
		Novice::DrawLine(int(points[i * 4 + 1].x), int(points[i * 4 + 1].y), int(points[i * 4 + 3].x), int(points[i * 4 + 3].y), color);
	}
	for (int i = 0; i < 4; i++) {
		Novice::DrawLine(int(points[i].x), int(points[i].y), int(points[i + 4].x), int(points[i + 4].y), color);
	}
}
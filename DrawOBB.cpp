#include "DrawOBB.h"
#include "Matrix4x4_operation.h"
#include <Novice.h>

void DrawOBB(const OBB& obb, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color) {
	
	Matrix4x4 obbWorldMatrix{
		.m{
			{obb.orientations[0].x	,obb.orientations[0].y	,obb.orientations[0].z	,0.0f},
			{obb.orientations[1].x	,obb.orientations[1].y	,obb.orientations[1].z	,0.0f},
			{obb.orientations[2].x	,obb.orientations[2].y	,obb.orientations[2].z	,0.0f},
			{obb.center.x			,obb.center.y			,obb.center.z			,1.0f},
		}
	};

	Vector3 vertices;
	Vector3 points[8];

	for (int i = 0; i < 8; i++) {
		//	X
		if (i < 4) {
			vertices.x = -obb.size.x;
		} else {
			vertices.x = +obb.center.x;
		}
		//	Y
		if ((i / 2) % 2 == 0) {
			vertices.y = -obb.size.y;
		} else {
			vertices.y = +obb.size.y;
		}
		//	Z
		if (i % 2 == 0) {
			vertices.z = -obb.size.z;
		} else {
			vertices.z = +obb.size.z;
		}

		points[i] = Transform(Transform(Transform(vertices, obbWorldMatrix), viewProjectionMatrix), viewportMatrix);
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
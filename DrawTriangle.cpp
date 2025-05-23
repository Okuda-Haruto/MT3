#include "DrawTriangle.h"
#include "Matrix4x4_operation.h"
#include "Novice.h"

void DrawTriangle(const Triangle& triangle, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color) {
	Vector3 vertices[3];
	for (int i = 0; i < 3; i++) {
		vertices[i] = Transform(Transform(triangle.vertices[i], viewProjectionMatrix), viewportMatrix);
	}

	Novice::DrawTriangle(int(vertices[0].x), int(vertices[0].y), int(vertices[1].x), int(vertices[1].y), int(vertices[2].x), int(vertices[2].y), color, kFillModeWireFrame);
}
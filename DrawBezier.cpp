#include "DrawBezier.h"
#include "DrawSphere.h"
#include "Vector3_operation.h"
#include "Matrix4x4_operation.h"
#include "Line.h"
#include <Novice.h>

void DrawBezier(
	const Vector3& controlPoint0,
	const Vector3& controlPoint1,
	const Vector3& controlPoint2,
	const Matrix4x4& viewProjectionMatrix,
	const Matrix4x4& viewportMatrix,
	uint32_t color) {

	for (uint8_t i = 0; i < 16; i++) {
		Segment segment;

		Vector3 p0p1 = Lerp(controlPoint0, controlPoint1, float(i) / 16);
		Vector3 p1p2 = Lerp(controlPoint1, controlPoint2, float(i) / 16);
		segment.origin = Lerp(p0p1, p1p2, float(i) / 16);

		p0p1 = Lerp(controlPoint0, controlPoint1, float(i + 1) / 16);
		p1p2 = Lerp(controlPoint1, controlPoint2, float(i + 1) / 16);
		segment.diff = Subtract(Lerp(p0p1, p1p2, float(i + 1) / 16), segment.origin);

		Vector3 start = Transform(Transform(segment.origin, viewProjectionMatrix), viewportMatrix);
		Vector3 end = Transform(Transform(Add(segment.origin,segment.diff), viewProjectionMatrix), viewportMatrix);

		Novice::DrawLine(int(start.x), int(start.y), int(end.x), int(end.y), color);
	}

	DrawSphere({ controlPoint0,0.01f }, viewProjectionMatrix, viewportMatrix, BLACK);
	DrawSphere({ controlPoint1,0.01f }, viewProjectionMatrix, viewportMatrix, BLACK);
	DrawSphere({ controlPoint2,0.01f }, viewProjectionMatrix, viewportMatrix, BLACK);
}
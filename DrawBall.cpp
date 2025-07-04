#include "DrawBall.h"
#include "DrawSphere.h"

void DrawBall(const Ball& ball, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix) {
	Sphere sphere{
		.center{ball.position},
		.radius{ball.radius}
	};

	DrawSphere(sphere, viewProjectionMatrix, viewportMatrix, ball.color);
}

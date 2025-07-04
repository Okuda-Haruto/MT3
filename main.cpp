#define NOMINMAX
#include <Novice.h>
#include <Vector3_operation.h>
#include <Matrix4x4_operation.h>

#include "DrawGrid.h"
#include "DrawSphere.h"
#include "DrawTriangle.h"
#include "DrawPlane.h"
#include "DrawAABB.h"
#include "DrawOBB.h"
#include "DrawBezier.h"
#include "DrawBall.h"
#include "Collision.h"

#define _USE_MATH_DEFINES
#include <cmath>
#include <imgui.h>
#include <numbers>

#include "Spring.h"
#include "Ball.h"
#include "Pendulum.h"
#include "ConicalPendulum.h"

#include "Reflect.h"

const char kWindowTitle[] = "LE2A_02_オクダハルト_MT3";

static const int kRowHeight = 20;
static const int kColumnWidth = 60;
void VectorScreenPrintf(int x, int y, const Vector3& vector, const char* label) {
	Novice::ScreenPrintf(x, y, "%.02f", vector.x);
	Novice::ScreenPrintf(x + kColumnWidth, y, "%.02f", vector.y);
	Novice::ScreenPrintf(x + kColumnWidth * 2, y, "%.02f", vector.z);
	Novice::ScreenPrintf(x + kColumnWidth * 3, y, "%s", label);
}
void MatrixScreenPrintf(int x, int y, const Matrix4x4& matrix,const char* label) {
	Novice::ScreenPrintf(x, y, "%s", label);
	for (int row = 0; row < 4; ++row) {
		for (int column = 0; column < 4; ++column) {
			Novice::ScreenPrintf(x + column * kColumnWidth, y + (row + 1) * kRowHeight, "%6.02f", matrix.m[row][column]);
		}
	}
}

static const int kWindowWidth = 1280;
static const int kWindowHeight = 720;

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, kWindowWidth, kWindowHeight);

	// キー入力結果を受け取る箱
	char keys[256] = {0};
	char preKeys[256] = {0};

	Vector3 rotate{0.0f,0.0f,0.0f};
	Vector3 translate{ 0.0f,0.0f,10.0f};
	Vector3 cameraTranslate{0.0f,3.0f,0.0f};
	Vector3 cameraRotate{ 0.26f,0.0f,0.0f };

	Plane plane;
	plane.normal = Normalize({ -0.2f,0.9f,-0.3f });
	plane.distance = 0.0f;

	Ball ball{};
	ball.position = { 0.8f,1.2f,0.3f };
	ball.mass = 2.0f;
	ball.radius = 0.05f;
	ball.color = WHITE;

	bool isStart = false;

	// ウィンドウの×ボタンが押されるまでループ
	while (Novice::ProcessMessage() == 0) {
		// フレームの開始
		Novice::BeginFrame();

		// キー入力を受け取る
		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);

		///
		/// ↓更新処理ここから
		///

		if (keys[DIK_W]) {
			translate.z += 0.1f;
		}
		if (keys[DIK_S]) {
			translate.z -= 0.1f;
		}
		if (keys[DIK_A]) {
			translate.x -= 0.1f;
		}
		if (keys[DIK_D]) {
			translate.x += 0.1f;
		}

		if (isStart) {
			//重力加速度
			ball.acceleration = { 0.0f,-9.8f,0.0f };

			float deltaTime = 1.0f / 60.0f;
			//反発係数
			float e = 0.8f;

			ball.velocity += ball.acceleration * deltaTime;

			Capsule capsule{
				.segment{
					.origin{ball.position},
					.diff{ball.velocity * deltaTime}
				},
				.radius{ball.radius}
			};
			ball.position += ball.velocity * deltaTime;
			if (IsCollision(capsule, plane)) {

				const float kMaxCollisionTime = 16;
				Vector3 CollisionPoint{};

				for (float collisionTime = 0; collisionTime < kMaxCollisionTime; collisionTime++) {
					if (IsCollision(Sphere{
						capsule.segment.origin + capsule.segment.diff * collisionTime / kMaxCollisionTime,
						capsule.radius },
						plane)) {
						//平面と点の距離
						float k = Dot(plane.normal, Vector3{ capsule.segment.origin + capsule.segment.diff * collisionTime / kMaxCollisionTime }) - plane.distance;
						//球の中心点から平面上におろした点
						Vector3 q = Vector3{ capsule.segment.origin + capsule.segment.diff * collisionTime / kMaxCollisionTime } - k * plane.normal;
						//qから半径分だけ平面から離した点
						CollisionPoint = q + (capsule.radius + 0.001f) * plane.normal;
						break;
					}
				}
				ball.position = CollisionPoint;

				Vector3 reflected = Reflect(ball.velocity, plane.normal);
				Vector3 projectToNormal = Project(reflected, plane.normal);
				Vector3 movingDirection = reflected - projectToNormal;
				ball.velocity = projectToNormal * e + movingDirection;
			}

		}

		Vector3 p{};


		ImGui::Begin("Window");
		ImGui::DragFloat3("CameraTranslate", &cameraTranslate.x,0.01f);
		ImGui::DragFloat3("CameraRotate", &cameraRotate.x,0.01f);
		if (ImGui::Button("Start")) {
			isStart = true;
		}
		ImGui::End();

		Matrix4x4 worldMatrix = MakeAffineMatrix({ 1.0f,1.0f,1.0f }, rotate, translate);
		Matrix4x4 cameraMatrix = MakeAffineMatrix({ 1.0f,1.0f,1.0f }, cameraRotate, cameraTranslate);
		Matrix4x4 viewMatrix = Inverse(cameraMatrix);
		Matrix4x4 projectionMatrix = MakePerspectiveFovMatrix(0.45f, float(kWindowWidth) / float(kWindowHeight), 0.1f, 100.0f);
		Matrix4x4 worldViewProjectionMatrix = Multiply(worldMatrix, Multiply(viewMatrix, projectionMatrix));
		Matrix4x4 viewportMatrix = MakeViewportMatrix(0, 0, float(kWindowWidth), float(kWindowHeight), 0.0f, 1.0f);

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		DrawGrid(worldViewProjectionMatrix, viewportMatrix);

		DrawPlane(plane, worldViewProjectionMatrix, viewportMatrix, WHITE);

		DrawBall(ball, worldViewProjectionMatrix, viewportMatrix);

		///
		/// ↑描画処理ここまで
		///

		// フレームの終了
		Novice::EndFrame();

		// ESCキーが押されたらループを抜ける
		if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0) {
			break;
		}
	}

	// ライブラリの終了
	Novice::Finalize();
	return 0;
}

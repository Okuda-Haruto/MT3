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
#include "Collision.h"

#define _USE_MATH_DEFINES
#include <cmath>
#include <imgui.h>

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

	Vector3 rotate{ 0.0f,0.0f,0.0f };
	Vector3 translate{ 0.0f,0.0f,10.0f};
	Vector3 cameraTranslate{0.0f,3.0f,0.0f};
	Vector3 cameraRotate{ 0.26f,0.0f,0.0f };

	Vector3 translates[3] = {
		{0.2f,1.0f,0.0f},
		{0.4f,0.0f,0.0f},
		{0.3f,0.0f,0.0f}
	};

	Vector3 rotates[3] = {
		{0.0f,0.0f,-6.8f},
		{0.0f,0.0f,-1.4f},
		{0.0f,0.0f,0.0f}
	};

	Vector3 scales[3] = {
		{1.0f,1.0f,1.0f},
		{1.0f,1.0f,1.0f},
		{1.0f,1.0f,1.0f}
	};

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

		ImGui::Begin("Window");
		ImGui::DragFloat3("CameraTranslate", &cameraTranslate.x,0.01f);
		ImGui::DragFloat3("CameraRotate", &cameraRotate.x,0.01f);
		ImGui::DragFloat3("translate 0", &translates[0].x, 0.01f);
		ImGui::DragFloat3("rotate 0", &rotates[0].x, 0.01f);
		ImGui::DragFloat3("scale 0", &scales[0].x, 0.01f);
		ImGui::DragFloat3("translate 1", &translates[1].x, 0.01f);
		ImGui::DragFloat3("rotate 1", &rotates[1].x, 0.01f);
		ImGui::DragFloat3("scale 1", &scales[1].x, 0.01f);
		ImGui::DragFloat3("translate 2", &translates[2].x, 0.01f);
		ImGui::DragFloat3("rotate 2", &rotates[2].x, 0.01f);
		ImGui::DragFloat3("scale 2", &scales[2].x, 0.01f);
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

		Matrix4x4 shoulderMatrix = MakeAffineMatrix(scales[0], rotates[0], translates[0]);
		Matrix4x4 elbowMatrix = MakeAffineMatrix(scales[1], rotates[1], translates[1]);
		Matrix4x4 handMatrix = MakeAffineMatrix(scales[2], rotates[2], translates[2]);

		Sphere sphere[3];
		sphere[0] = {
			.center{Transform({0.0f,0.0f,0.0f},shoulderMatrix)},
			.radius{0.1f}
		};
		sphere[1] = {
			.center{Transform({0.0f,0.0f,0.0f},Multiply(elbowMatrix,shoulderMatrix))},
			.radius{0.1f}
		};
		sphere[2] = {
			.center{Transform({0.0f,0.0f,0.0f},Multiply(handMatrix,Multiply(elbowMatrix,shoulderMatrix)))},
			.radius{0.1f}
		};

		Vector3 start = Transform(Transform(sphere[0].center, worldViewProjectionMatrix), viewportMatrix);
		Vector3 end = Transform(Transform(sphere[1].center, worldViewProjectionMatrix), viewportMatrix);

		Novice::DrawLine(int(start.x), int(start.y), int(end.x), int(end.y), WHITE);

		start = end;
		end = Transform(Transform(sphere[2].center, worldViewProjectionMatrix), viewportMatrix);

		Novice::DrawLine(int(start.x), int(start.y), int(end.x), int(end.y), WHITE);

		DrawSphere(sphere[0], worldViewProjectionMatrix, viewportMatrix, RED);
		DrawSphere(sphere[1], worldViewProjectionMatrix, viewportMatrix, GREEN);
		DrawSphere(sphere[2], worldViewProjectionMatrix, viewportMatrix, BLUE);

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

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
	//ローカル座標系の頂点
	Vector3 kLocalVertices[3] = {
		{1.0f,-1.0f,0.0f},
		{0.0f,1.0f,0.0f},
		{-1.0f,-1.0f,0.0f}
	};

	OBB obb{
		.center{1.0f,1.0f,1.0f},
		.orientations{
			{0.0f,0.0f,0.0f},
			{0.0f,0.0f,0.0f},
			{0.0f,0.0f,0.0f}
		},
		.size{1.0f,1.0f,1.0f}
	};

	Sphere sphere{
		.center{2.0f,2.0f,2.0f},
		.radius{1.0f}
	};

	Vector3 obbRotate{};

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
		ImGui::DragFloat3("obb center", &obb.center.x, 0.01f);
		ImGui::SliderAngle("obb rotate X", &obbRotate.x, 0.01f);
		ImGui::SliderAngle("obb rotate Y", &obbRotate.y, 0.01f);
		ImGui::SliderAngle("obb rotate Z", &obbRotate.z, 0.01f);
		ImGui::DragFloat3("obb size", &obb.size.x, 0.01f);
		ImGui::DragFloat3("sphere center", &sphere.center.x, 0.01f);
		ImGui::DragFloat("sphere radius", &sphere.radius, 0.01f);
		ImGui::End();


		Matrix4x4 rotateX = MakeRotateXMatrix(obbRotate.x);
		Matrix4x4 rotateY = MakeRotateYMatrix(obbRotate.y);
		Matrix4x4 rotateZ = MakeRotateZMatrix(obbRotate.z);

		Matrix4x4 rotateMatrix = Multiply(rotateX, Multiply(rotateY, rotateZ));
		obb.orientations[0] = { rotateMatrix.m[0][0],rotateMatrix.m[1][0] ,rotateMatrix.m[2][0] };
		obb.orientations[1] = { rotateMatrix.m[0][1],rotateMatrix.m[1][1] ,rotateMatrix.m[2][1] };
		obb.orientations[2] = { rotateMatrix.m[0][2],rotateMatrix.m[1][2] ,rotateMatrix.m[2][2] };


		Matrix4x4 worldMatrix = MakeAffineMatrix({ 1.0f,1.0f,1.0f }, rotate, translate);
		Matrix4x4 cameraMatrix = MakeAffineMatrix({ 1.0f,1.0f,1.0f }, cameraRotate, cameraTranslate);
		Matrix4x4 viewMatrix = Inverse(cameraMatrix);
		Matrix4x4 projectionMatrix = MakePerspectiveFovMatrix(0.45f, float(kWindowWidth) / float(kWindowHeight), 0.1f, 100.0f);
		Matrix4x4 worldViewProjectionMatrix = Multiply(worldMatrix, Multiply(viewMatrix, projectionMatrix));
		Matrix4x4 viewportMatrix = MakeViewportMatrix(0, 0, float(kWindowWidth), float(kWindowHeight), 0.0f, 1.0f);
		Vector3 screenVertices[3];
		for (int i = 0; i < 3; i++) {
			Vector3 ndcVertex = Transform(kLocalVertices[i], worldViewProjectionMatrix);
			screenVertices[i] = Transform(ndcVertex, viewportMatrix);
		}

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		DrawGrid(worldViewProjectionMatrix, viewportMatrix);

		DrawSphere(sphere, worldViewProjectionMatrix, viewportMatrix, WHITE);

		if (IsCollision(obb,sphere)) {
			DrawOBB(obb, worldViewProjectionMatrix, viewportMatrix, RED);
		} else {
			DrawOBB(obb, worldViewProjectionMatrix, viewportMatrix, WHITE);
		}

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

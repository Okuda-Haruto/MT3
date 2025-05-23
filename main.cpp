#define NOMINMAX
#include <Novice.h>
#include <Vector3_operation.h>
#include <Matrix4x4_operation.h>

#include "DrawGrid.h"
#include "DrawSphere.h"
#include "DrawTriangle.h"
#include "DrawPlane.h"
#include "DrawAABB.h"
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

	AABB aabb1{
		{-0.5f,-0.5f,-0.5f},
		{0.0f,0.0f,0.0f}
	};
	AABB aabb2{
		{0.2f,0.2f,0.2f},
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
		ImGui::DragFloat3("aabb1 min", &aabb1.min.x, 0.01f);
		ImGui::DragFloat3("aabb1 max", &aabb1.max.x, 0.01f);
		ImGui::DragFloat3("aabb2 min", &aabb2.min.x, 0.01f);
		ImGui::DragFloat3("aabb2 max", &aabb2.max.x, 0.01f);
		ImGui::End();

		aabb1.min.x = std::min(aabb1.min.x, aabb1.max.x);
		aabb1.max.x = std::max(aabb1.min.x, aabb1.max.x);
		aabb1.min.y = std::min(aabb1.min.y, aabb1.max.y);
		aabb1.max.y = std::max(aabb1.min.y, aabb1.max.y);
		aabb1.min.z = std::min(aabb1.min.z, aabb1.max.z);
		aabb1.max.z = std::max(aabb1.min.z, aabb1.max.z);

		aabb2.min.x = std::min(aabb2.min.x, aabb2.max.x);
		aabb2.max.x = std::max(aabb2.min.x, aabb2.max.x);
		aabb2.min.y = std::min(aabb2.min.y, aabb2.max.y);
		aabb2.max.y = std::max(aabb2.min.y, aabb2.max.y);
		aabb2.min.z = std::min(aabb2.min.z, aabb2.max.z);
		aabb2.max.z = std::max(aabb2.min.z, aabb2.max.z);

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

		if (IsCollision(aabb1,aabb2)) {
			DrawAABB(aabb1, worldViewProjectionMatrix, viewportMatrix, RED);
		} else {
			DrawAABB(aabb1, worldViewProjectionMatrix, viewportMatrix, WHITE);
		}

		DrawAABB(aabb2, worldViewProjectionMatrix, viewportMatrix, WHITE);

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

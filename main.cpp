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
#include "Quaternion.h"

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
			Novice::ScreenPrintf(x + column * kColumnWidth, y + (row + 1) * kRowHeight, "%6.03f", matrix.m[row][column]);
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

	Quaternion q1 = { 2,3,4,1 };
	Quaternion q2 = { 1,3,5,2 };
	Quaternion identity = IdentityQuaternion();
	Quaternion conj = Conjugate(q1);
	Quaternion inv = Inverse(q1);
	Quaternion normal = Normalize(q1);
	Quaternion mul1 = Multiply(q1, q2);
	Quaternion mul2 = Multiply(q2, q1);
	float norm = Norm(q1);

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

		ImGui::Begin("Quaternion");
		ImGui::DragFloat4("Identity",&identity.x);
		ImGui::DragFloat4("Conjugate", &conj.x);
		ImGui::DragFloat4("Inverse", &inv.x);
		ImGui::DragFloat4("Normalize", &normal.x);
		ImGui::DragFloat4("Multiply1", &mul1.x);
		ImGui::DragFloat4("Multiply2", &mul2.x);
		ImGui::DragFloat("Norm", &norm);
		ImGui::End();


		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		

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

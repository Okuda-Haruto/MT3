#include "DrawGrid.h"
#include "Matrix4x4_operation.h"
#include <Novice.h>
#include <format>

void DrawGrid(const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix) {
	const float kGridHalfWidth = 2.0f;										//Gridの半分の幅
	const uint32_t kSubdivision = 10;										//分割数
	const float kGridEvery = (kGridHalfWidth * 2.0f) / float(kSubdivision);	//1つ分の長さ
	//奥から手前への線を順々に引いていく
	for (uint32_t xIndex = 0; xIndex <= kSubdivision; ++xIndex) {

		//上の情報を使ってワールド座標系上の始点と終点を求める
		Vector3 kLocalVertices[2] = {
		{kGridEvery * xIndex - kGridHalfWidth,0.0f,-kGridHalfWidth},
		{kGridEvery * xIndex - kGridHalfWidth,0.0f,kGridHalfWidth},
		};

		//スクリーン座標系まで変換をかける
		Vector3 screenVertices[2];
		 for (int i = 0; i < 2; i++) {
			Vector3 ndcVertex = Transform(kLocalVertices[i], viewProjectionMatrix);
			screenVertices[i] = Transform(ndcVertex, viewportMatrix);
		}

		//スクリーン変換した座標系を使って表示
		if (xIndex == kSubdivision / 2) {
			Novice::DrawLine(int(screenVertices[0].x), int(screenVertices[0].y), int(screenVertices[1].x), int(screenVertices[1].y), 0x000000FF);
		} else {
			Novice::DrawLine(int(screenVertices[0].x), int(screenVertices[0].y), int(screenVertices[1].x), int(screenVertices[1].y), 0xAAAAAAFF);
		}
	}

	//左から右への線を順々に引いていく
	for (uint32_t zIndex = 0; zIndex <= kSubdivision; ++zIndex) {
		//上の情報を使ってワールド座標系上の始点と終点を求める
		Vector3 kLocalVertices[2] = {
		{-kGridHalfWidth,0.0f,kGridEvery * zIndex - kGridHalfWidth },
		{kGridHalfWidth,0.0f,kGridEvery * zIndex - kGridHalfWidth },
		};

		//スクリーン座標系まで変換をかける
		Vector3 screenVertices[2];
		for (int i = 0; i < 2; i++) {
			Vector3 ndcVertex = Transform(kLocalVertices[i], viewProjectionMatrix);
			screenVertices[i] = Transform(ndcVertex, viewportMatrix);
		}

		//スクリーン変換した座標系を使って表示
		if (zIndex == kSubdivision / 2) {
			Novice::DrawLine(int(screenVertices[0].x), int(screenVertices[0].y), int(screenVertices[1].x), int(screenVertices[1].y), 0x000000FF);
		} else {
			Novice::DrawLine(int(screenVertices[0].x), int(screenVertices[0].y), int(screenVertices[1].x), int(screenVertices[1].y), 0xAAAAAAFF);
		}
	}
}
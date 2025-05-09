#include "DrawSphere.h"
#include <Novice.h>
#include "Matrix4x4_operation.h"

void DrawSphere(const Sphere& sphere, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color) {
	const uint32_t kSubdivision = 16;
	const float kLonEvery = float(M_PI) * 2.0f / kSubdivision;
	const float kLatEvery = float(M_PI) / kSubdivision;
	//緯度方向に分割 -π/2 ~ π/2
	for (uint32_t latIndex = 0; latIndex < kSubdivision; ++latIndex) {
		float lat = -float(M_PI) / 2.0f + kLatEvery * latIndex;	//現在の緯度
		//緯度の方向に分割 0 ~ 2π
		for (uint32_t lonIndex = 0; lonIndex < kSubdivision; ++lonIndex) {
			float lon = lonIndex * kLonEvery;	//現在の緯度
			//World座標系でのa,b,cを求める
			Vector3 a, b, c;
			a.x = std::cos(lat) * std::cos(lon) * sphere.radius + sphere.center.x;
			a.y = std::sin(lat) * sphere.radius + sphere.center.y;
			a.z = std::cos(lat) * std::sin(lon) * sphere.radius + sphere.center.z;

			b.x = std::cos(lat + kLatEvery) * std::cos(lon) * sphere.radius + sphere.center.x;
			b.y = std::sin(lat + kLatEvery) * sphere.radius + sphere.center.y;
			b.z = std::cos(lat + kLatEvery) * std::sin(lon) * sphere.radius + sphere.center.z;

			c.x = std::cos(lat) * std::cos(lon + kLonEvery) * sphere.radius + sphere.center.x;
			c.y = std::sin(lat) * sphere.radius + sphere.center.y;
			c.z = std::cos(lat) * std::sin(lon + kLonEvery) * sphere.radius + sphere.center.z;

			//スクリーン座標系まで変換をかける
			Vector3 ndcVertex = Transform(a, viewProjectionMatrix);
			Vector3 worldA = Transform(ndcVertex, viewportMatrix);
			ndcVertex = Transform(b, viewProjectionMatrix);
			Vector3 worldB = Transform(ndcVertex, viewportMatrix);
			ndcVertex = Transform(c, viewProjectionMatrix);
			Vector3 worldC = Transform(ndcVertex, viewportMatrix);

			//ab,acで線を引く
			Novice::DrawLine(int(worldA.x), int(worldA.y), int(worldB.x), int(worldB.y), color);
			Novice::DrawLine(int(worldA.x), int(worldA.y), int(worldC.x), int(worldC.y), color);
		}
	}
}
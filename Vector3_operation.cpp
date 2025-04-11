﻿#include<Vector3_operation.h>
#include<math.h>

//加算
Vector3 Add(const Vector3& v1, const Vector3& v2) {
	Vector3 AnswerVector;
	AnswerVector.x = v1.x + v2.x;
	AnswerVector.y = v1.y + v2.y;
	AnswerVector.z = v1.z + v2.z;
	return AnswerVector;
}
//減算
Vector3 Subtract(const Vector3& v1, const Vector3& v2) {
	Vector3 AnswerVector;
	AnswerVector.x = v1.x - v2.x;
	AnswerVector.y = v1.y - v2.y;
	AnswerVector.z = v1.z - v2.z;
	return AnswerVector;
}
//スカラー倍
Vector3 Multiply(float scalar, const Vector3& v) {
	Vector3 AnswerVector;
	AnswerVector.x = scalar * v.x;
	AnswerVector.y = scalar * v.y;
	AnswerVector.z = scalar * v.z;
	return AnswerVector;
}
//内積
float Dot(const Vector3& v1, const Vector3& v2) {
	float AnswerFloat;
	AnswerFloat = v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
	return AnswerFloat;
}
//長さ(ノルム)
float Length(const Vector3& v) {
	float AnswerFloat;
	AnswerFloat = sqrtf(Dot(v,v));
	return AnswerFloat;
}
//正規化
Vector3 Normalize(const Vector3& v) {
	Vector3 AnswerVector;
	AnswerVector = Multiply(1.0f / Length(v),v);
	return AnswerVector;
}
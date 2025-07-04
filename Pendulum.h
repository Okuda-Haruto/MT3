#pragma once
#include "Vector3.h"

struct Pendulum {
	Vector3 anchor;				//アンカーポイント。固定された端の位置
	float length;				//紐の長さ
	float angle;				//現在の角度
	float angularVelocity;		//角速度ω
	float angularAcceleration;	//角加速度
};
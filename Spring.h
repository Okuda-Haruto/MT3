#pragma once
#include "Vector3.h"

struct Spring {
	Vector3 anchor;				//アンカー。固定された端の位置
	float naturalLength;		//自然長
	float stiffness;			//剛体。バネ定数k
	float dampingCoefficient;	//減衰係数
};
#pragma once
#include "Vector3D.h"

struct Ray
{
	Ray();
	explicit Ray(const Vector3D& origin, const Vector3D& direction, float time);

	Vector3D GetPositionFromParameter(float t) const;

	Vector3D Origin, Direction;
	float Time;
};


#pragma once
#include "Vector3D.h"

struct Ray
{
	Ray();
	Ray(const Vector3D& origin, const Vector3D& direction);

	Vector3D GetPositionFromParameter(float t) const;

	Vector3D Origin, Direction;
};


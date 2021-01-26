#pragma once
#include "Ray.h"

class Camera
{
public:
	Camera();

	Ray GetRayAtScreenUV(float u, float v) const;

	Vector3D ViewBase;
	Vector3D BaseU;
	Vector3D BaseV;
	Vector3D Position;
};


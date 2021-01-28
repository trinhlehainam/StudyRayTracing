#pragma once
#include "Ray.h"

class Camera
{
public:
	Camera();
	explicit Camera(Position3 lookFrom, Position3 lookAt, Vector3D vectorUp, float FOV, float aspectRatio);

	Ray GetRayAtScreenUV(float u, float v) const;

	Position3 Position;
	Position3 ViewPosition;
	Vector3D ViewU;
	Vector3D ViewV;
};


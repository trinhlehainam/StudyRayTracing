#pragma once
#include "Ray.h"

class Camera
{
public:
	Camera();
	explicit Camera(Position3 lookFrom, Position3 lookAt, Vector3D vectorUp, float FOV, float aspectRatio,
	float aperture, float focusDistance);

	Ray GetRayAtScreenUV(float u, float v) const;

	// Camera space unit vectors
	Vector3D W;
	Vector3D U;
	Vector3D V;

	Position3 Position;
	Position3 ViewPosition;
	Vector3D ViewU;
	Vector3D ViewV;

	float LenRadius;
};


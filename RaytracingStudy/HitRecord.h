#pragma once
#include <memory>

#include "Materials/IMaterial.h"
#include "Common/Ray.h"

struct HitRecord
{
	Vector3D Position;		// Hit position
	Vector3D Normal;
	float t;				// Variable to calculate the position from a ray
	bool FrontFace;			// checking the ray is bouncing from outside or insdie of object
	std::shared_ptr<IMaterial> pMaterial;

	void SetFaceNormal(const Ray& ray, const Vector3D& outwardNomarl);
};


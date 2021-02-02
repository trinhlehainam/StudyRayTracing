#pragma once
#include <memory>

#include "Materials/IMaterial.h"
#include "Common/Ray.h"

struct HitRecord
{
	Vector3D Position;		// Hit position
	Vector3D Normal;
	bool FrontFace;			// checking the ray is bouncing from outside or insdie of object
	float U, V;
	float t;				// Variable to calculate the position from a ray
	std::shared_ptr<IMaterial> pMaterial;

	void SetFaceNormal(const Ray& ray, const Vector3D& outwardNomarl);
};


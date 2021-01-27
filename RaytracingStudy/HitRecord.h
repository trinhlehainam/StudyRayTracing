#pragma once
#include <memory>

#include "Material.h"
#include "Common/Ray.h"

struct HitRecord
{
	void SetFaceNormal(const Ray& ray, const Vector3D& outwardNomarl);

	float t;
	Vector3D Position;
	Vector3D Normal;
	bool FrontFace;
	std::shared_ptr<Material> pMaterial;
};


#pragma once
#include <memory>

#include "Material.h"
#include "Common/Ray.h"

struct HitRecord
{
	Vector3D Position;
	Vector3D Normal;
	float t;
	bool FrontFace;
	std::shared_ptr<Material> pMaterial;

	void SetFaceNormal(const Ray& ray, const Vector3D& outwardNomarl);
};


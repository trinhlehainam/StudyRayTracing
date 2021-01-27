#pragma once
#include "Material.h"

#include "Common/Vector3D.h"

class Lambertian : public Material
{
public:
	explicit Lambertian(const Vector3D& albedo);
	virtual bool ScatterRay(const Ray& inRay, const HitRecord& record, Vector3D& attenuation, Ray& scatteredRay) const override;

public:
	Vector3D Albedo;
};


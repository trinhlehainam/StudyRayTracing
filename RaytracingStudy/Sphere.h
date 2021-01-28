#pragma once
#include "IHitable.h"

#include <memory>

#include "Common/Ray.h"
#include "Materials/IMaterial.h"

class Sphere : public IHitable
{
public:
	explicit Sphere(const std::shared_ptr<IMaterial>& material);
	explicit Sphere(const Vector3D& center, float radius, const std::shared_ptr<IMaterial>& material);

	virtual bool IsHit(const Ray& ray, float minRange, float maxRange, HitRecord& record) const override;

	Vector3D Center;
	float Radius;
	std::shared_ptr<IMaterial> pMaterial;
};


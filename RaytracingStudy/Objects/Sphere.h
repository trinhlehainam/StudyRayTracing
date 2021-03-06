#pragma once
#include "IHitable.h"

#include <memory>

#include "../Common/Ray.h"


class IMaterial;

class Sphere : public IHitable
{
public:
	explicit Sphere(const std::shared_ptr<IMaterial>& material);
	explicit Sphere(const Vector3D& center, float radius, const std::shared_ptr<IMaterial>& material);

	virtual bool IsHit(const Ray& ray, float minRange, float maxRange, HitRecord& record) const override;
	virtual bool IsBoundingBox(AABB& output) const;

	void GetSphereUV(const Position3& point, float& u, float& v) const;

	Vector3D Center;
	float Radius;
	std::shared_ptr<IMaterial> pMaterial;
};


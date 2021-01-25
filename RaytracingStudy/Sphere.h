#pragma once
#include "IHitable.h"

class Sphere : public IHitable
{
public:
	Sphere();
	explicit Sphere(const Vector3D& center, float radius);

	virtual bool CheckHit(const Ray& ray, float minRange, float maxRange, HitRecord& record) const override;

	Vector3D Center;
	float Radius;
};


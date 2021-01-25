#pragma once

#include "Ray.h"

struct HitRecord
{
	float t;
	Vector3D Position;
	Vector3D Normal;
};

class IHitable
{
public:
	virtual ~IHitable() {};
	virtual bool CheckHit(const Ray& ray, float minRange, float maxRange, HitRecord& record) const = 0;
};

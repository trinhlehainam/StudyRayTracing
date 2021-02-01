#pragma once

struct HitRecord;
struct Ray;
struct AABB;

class IHitable
{
public:
	virtual ~IHitable() {};
	virtual bool IsHit(const Ray& ray, float rangeMin, float rangeMax, HitRecord& record) const = 0;
	virtual bool IsBoundingBox(AABB& output) const = 0;
};

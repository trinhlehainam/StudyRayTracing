#pragma once

struct HitRecord;
struct Ray;

class IHitable
{
public:
	virtual ~IHitable() {};
	virtual bool CheckHit(const Ray& ray, float minRange, float maxRange, HitRecord& record) const = 0;
};

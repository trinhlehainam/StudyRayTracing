#pragma once
#include "IHitable.h"

#include <memory>
#include <vector>

#include "../AABB.h"

class HitableList;

// Bounding volume hierarchy
class BVHNode : public IHitable
{
public:
	explicit BVHNode(const HitableList& list);
	explicit BVHNode(const std::vector<std::shared_ptr<IHitable>>& srcObjects, size_t start,
		size_t end);

	virtual bool IsHit(const Ray& ray, float rangeMin, float rangeMax, HitRecord& record) const override;

	virtual bool IsBoundingBox(AABB& output) const override;

	std::shared_ptr<IHitable> Left;
	std::shared_ptr<IHitable> Right;
	AABB Volume;
};


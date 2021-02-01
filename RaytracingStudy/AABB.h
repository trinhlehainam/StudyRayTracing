#pragma once
#include "Common/Ray.h"

// Axis-aligned bounding box
struct AABB
{
	AABB();
	explicit AABB(const Position3& min, const Position3& max);

	bool IsHit(const Ray& ray, float rangeMin, float rangeMax) const;

	// Create a box that wrap both boxes
	static AABB SurroundingBox(const AABB& a, const AABB& b);

	Position3 Min, Max;
};




#pragma once
#include "IHitable.h"

#include <memory>

#include "Common/Ray.h"
#include "Materials/IMaterial.h"

class MovingSphere : public IHitable
{
public:
	explicit MovingSphere(const Position3& center1, const Position3& center2, float radius,
		const std::shared_ptr<IMaterial>& material);

	virtual bool IsHit(const Ray& ray, float minRange, float maxRange, HitRecord& record) const override;
	virtual bool IsBoundingBox(AABB& output) const;
	Position3 GetCenter(float time) const;

	Position3 Center1, Center2;
	float Radius;
	std::shared_ptr<IMaterial> pMaterial;
};


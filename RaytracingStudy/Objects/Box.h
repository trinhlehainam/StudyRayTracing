#pragma once
#include "IHitable.h"

#include <memory>

#include "../Common/Vector3D.h"
#include "HitableList.h"

class IMaterial;

class Box :
    public IHitable
{
public:
    explicit Box(const Position3& min, const Position3& max, std::shared_ptr<IMaterial> matterial);

    virtual bool IsHit(const Ray& ray, float rangeMin, float rangeMax, HitRecord& record) const override;

    virtual bool IsBoundingBox(AABB& output) const override;

    Position3 Min, Max;
    HitableList Sides;
};


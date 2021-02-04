#pragma once
#include "../Objects/IHitable.h"

#include <memory>

#include "../Common/Vector3D.h"

class Translate :
    public IHitable
{
public:
    explicit Translate(std::shared_ptr<IHitable> object, const Vector3D& offset);

    virtual bool IsHit(const Ray& ray, float rangeMin, float rangeMax, HitRecord& record) const override;

    virtual bool IsBoundingBox(AABB& output) const override;

    std::shared_ptr<IHitable> pObject;
    Vector3D Offset;
};


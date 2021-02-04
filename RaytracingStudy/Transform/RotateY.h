#pragma once
#include "../Objects/IHitable.h"

#include <memory>

#include "../Common/Vector3D.h"
#include "../AABB.h"

class RotateY :
    public IHitable
{
public:
    explicit RotateY(std::shared_ptr<IHitable> object, float angle);

    virtual bool IsHit(const Ray& ray, float rangeMin, float rangeMax, HitRecord& record) const override;

    virtual bool IsBoundingBox(AABB& output) const override;

    std::shared_ptr<IHitable> pObject;
    float CosTheta;
    float SinTheta;
    AABB Box;
    bool IsBB;
};


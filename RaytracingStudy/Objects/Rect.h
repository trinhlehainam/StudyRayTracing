#pragma once
#include "IHitable.h"

#include <memory>

class IMaterial;

class XY_Rect :
    public IHitable
{
public:
    explicit XY_Rect(float x0, float x1, float y0, float y1, float k, std::shared_ptr<IMaterial>);

    virtual bool IsHit(const Ray& ray, float rangeMin, float rangeMax, HitRecord& record) const override;
    virtual bool IsBoundingBox(AABB& output) const override;

    float X0, X1, Y0, Y1, K;
    std::shared_ptr<IMaterial> pMaterial;
};


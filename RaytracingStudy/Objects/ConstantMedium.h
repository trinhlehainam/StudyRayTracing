#pragma once
#include "IHitable.h"

#include <memory>

class IMaterial;
struct Vector3D;
using Color3 = Vector3D;

class ConstantMedium :
    public IHitable
{
public:
    explicit ConstantMedium(std::shared_ptr<IHitable> boundary, std::shared_ptr<IMaterial> material, float density);
    explicit ConstantMedium(std::shared_ptr<IHitable> boundary, const Color3& color, float density);

    virtual bool IsHit(const Ray& ray, float rangeMin, float rangeMax, HitRecord& record) const override;

    virtual bool IsBoundingBox(AABB& output) const override;

public:
    std::shared_ptr<IHitable> Boundary;
    std::shared_ptr<IMaterial> PhaseFunction;
    float Inv_Neg_Density;
};


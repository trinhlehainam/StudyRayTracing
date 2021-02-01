#pragma once
#include "IHitable.h"

#include <vector>
#include <memory>

class HitableList :
    public IHitable
{
public:
    HitableList();
    explicit HitableList(std::shared_ptr<IHitable> object);
    ~HitableList();

    virtual bool IsHit(const Ray& ray, float minRange, float maxRange, HitRecord& record) const override;
    virtual bool IsBoundingBox(AABB& output) const;

    std::vector<std::shared_ptr<IHitable>> Objects;

};


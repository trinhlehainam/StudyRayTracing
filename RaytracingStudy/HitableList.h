#pragma once
#include "IHitable.h"

#include <vector>
#include <memory>

class HitableList :
    public IHitable
{
public:
    HitableList();
    ~HitableList();

    virtual bool CheckHit(const Ray& ray, float minRange, float maxRange, HitRecord& record) const override;

    std::vector<std::shared_ptr<IHitable>> List;

};


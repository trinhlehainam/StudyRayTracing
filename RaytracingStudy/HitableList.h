#pragma once
#include "IHitable.h"

class HitableList :
    public IHitable
{
public:
    explicit HitableList(IHitable** pList, unsigned int objectCount);
    ~HitableList();

    virtual bool CheckHit(const Ray& ray, float minRange, float maxRange, HitRecord& record) const override;

    IHitable** List = nullptr;
    unsigned int Count = 0;

};


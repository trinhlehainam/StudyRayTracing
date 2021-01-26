#include "HitableList.h"
#include "HitRecord.h"

HitableList::HitableList()
{
}

HitableList::~HitableList()
{
}

bool HitableList::CheckHit(const Ray& ray, float minRange, float maxRange, HitRecord& record) const
{
    HitRecord tempRect;
    bool check = false;
    float closestRange = maxRange;
    for (auto& hitable : List)
    {
        if (hitable->CheckHit(ray, minRange, closestRange, tempRect))
        {
            check = true;
            closestRange = tempRect.t;
            record = tempRect;
        }
    }
    return check;
}

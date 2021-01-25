#include "HitableList.h"

HitableList::HitableList(IHitable** pList, unsigned int objectCount):List(pList), Count(objectCount)
{

}

HitableList::~HitableList()
{
    if (List != nullptr)
    {
        for (unsigned int i = 0; i < Count; ++i)
        {
            delete List[i];
        }
        List = nullptr;
        Count = 0;
    }
}

bool HitableList::CheckHit(const Ray& ray, float minRange, float maxRange, HitRecord& record) const
{
    HitRecord tempRect;
    bool check = false;
    float closestRange = maxRange;
    for (unsigned int i = 0; i < Count; ++i)
    {
        if (List[i]->CheckHit(ray, minRange, closestRange, tempRect))
        {
            check = true;
            closestRange = tempRect.t;
            record = tempRect;
        }
    }
    return check;
}

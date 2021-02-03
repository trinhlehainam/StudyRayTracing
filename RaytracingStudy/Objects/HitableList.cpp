#include "HitableList.h"

#include "../HitRecord.h"
#include "../AABB.h"

HitableList::HitableList()
{
}

HitableList::HitableList(std::shared_ptr<IHitable> object)
{
    Objects.push_back(object);
}

HitableList::~HitableList()
{
}

bool HitableList::IsHit(const Ray& ray, float minRange, float maxRange, HitRecord& record) const
{
    HitRecord tempRect;
    bool check = false;
    float closestRange = maxRange;
    for (auto& hitable : Objects)
    {
        if (hitable->IsHit(ray, minRange, closestRange, tempRect))
        {
            check = true;
            closestRange = tempRect.t;
            record = tempRect;
        }
    }
    return check;
}

bool HitableList::IsBoundingBox(AABB& output) const
{
    if (Objects.empty()) return false;

    AABB temp;
    unsigned int index = 0;

    for (const auto& object : Objects)
    {
        if (!object->IsBoundingBox(temp)) return false;
        output = index == 0 ? temp : AABB::SurroundingBox(output, temp);
        ++index;
    }

    return true;
}

void HitableList::Add(std::shared_ptr<IHitable> object)
{
    Objects.push_back(std::move(object));
}

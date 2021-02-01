#include "AABB.h"

#include <algorithm>

#include "Common/MathHelper.h"

AABB::AABB()
{
}

AABB::AABB(const Position3& min, const Position3& max):Min(min),Max(max)
{
}

bool AABB::IsHit(const Ray& ray, float rangeMin, float rangeMax) const
{
    for (unsigned int i = 0; i < 3; ++i)
    {
        float invDir = 1.0f / ray.Direction[i];
        float timeIn = (Min[i] - ray.Origin[i]) * invDir;
        float timeOut = (Max[i] - ray.Origin[i]) * invDir;
        // Check direction
        if (invDir < 0.0f) std::swap(timeIn, timeOut);
        // Find the LAST INTERSECT time
        rangeMin = timeIn > rangeMin ? timeIn : rangeMin;
        // Find the FIRST RELEASE time
        rangeMax = timeOut < rangeMax ? timeOut : rangeMax;
        if (rangeMax <= rangeMin)
            return false;
    }

    return true;
}

AABB AABB::SurroundingBox(const AABB& a, const AABB& b)
{
    Position3 min(MathHelper::Min(a.Min.X, b.Min.X),
                MathHelper::Min(a.Min.Y, b.Min.Y),
                MathHelper::Min(a.Min.Z, b.Min.Z));
    Position3 max(MathHelper::Max(a.Max.X, b.Max.X),
                MathHelper::Max(a.Max.Y, b.Max.Y),
                MathHelper::Max(a.Max.Z, b.Max.Z));
    return AABB(min, max);
}

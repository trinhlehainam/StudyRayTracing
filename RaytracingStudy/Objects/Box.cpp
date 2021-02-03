#include "Box.h"

#include "../Common/Ray.h"
#include "Rect.h"
#include "../AABB.h"
#include "../HitRecord.h"

Box::Box(const Position3& min, const Position3& max, std::shared_ptr<IMaterial> matterial):
    Min(min),Max(max)
{
    Sides.Add(std::make_shared<XY_Rect>(min.X, max.X, min.Y, max.Y, min.Z, matterial));
    Sides.Add(std::make_shared<XY_Rect>(min.X, max.X, min.Y, max.Y, max.Z, matterial));
         
    Sides.Add(std::make_shared<YZ_Rect>(min.Y, max.Y, min.Z, max.Z, min.X, matterial));
    Sides.Add(std::make_shared<YZ_Rect>(min.Y, max.Y, min.Z, max.Z, max.X, matterial));
         
    Sides.Add(std::make_shared<XZ_Rect>(min.X, max.X, min.Z, max.Z, min.Y, matterial));
    Sides.Add(std::make_shared<XZ_Rect>(min.X, max.X, min.Z, max.Z, max.Y, matterial));
}

bool Box::IsHit(const Ray& ray, float rangeMin, float rangeMax, HitRecord& record) const
{
    return Sides.IsHit(ray, rangeMin, rangeMax, record);
}

bool Box::IsBoundingBox(AABB& output) const
{
    output.Min = Min;
    output.Max = Max;

    return true;
}

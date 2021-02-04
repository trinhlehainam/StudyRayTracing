#include "Translate.h"

#include "../Common/Ray.h"
#include "../AABB.h"
#include "../HitRecord.h"

Translate::Translate(std::shared_ptr<IHitable> object, const Vector3D& offset):
    Offset(offset),pObject(object)
{
}

bool Translate::IsHit(const Ray& ray, float rangeMin, float rangeMax, HitRecord& record) const
{
    Ray moved_ray(ray.Origin - Offset, ray.Direction, ray.Time);
    if (!pObject->IsHit(moved_ray, rangeMin, rangeMax, record))
        return false;
    record.Position += Offset;
    return true;
}

bool Translate::IsBoundingBox(AABB& output) const
{
    if (!pObject->IsBoundingBox(output)) return false;

    output.Min += Offset;
    output.Max += Offset;

    return true;
}

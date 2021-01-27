#include "Metal.h"

#include "Common/Ray.h"
#include "HitRecord.h"

Metal::Metal(const Vector3D& albedo):Albedo(albedo)
{
}

bool Metal::ScatterRay(const Ray& rayIn, const HitRecord& record, Vector3D& attenuation, Ray& rayScatter) const
{
    attenuation = Albedo;
    rayScatter.Origin = record.Position;
    rayScatter.Direction = ReflectedVector(rayIn.Direction, record.Normal);

    return Dot(rayScatter.Direction, record.Normal) > 0;
}

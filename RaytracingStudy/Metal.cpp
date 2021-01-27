#include "Metal.h"

#include "Common/Ray.h"
#include "Common/MathHelper.h"
#include "HitRecord.h"

Metal::Metal(const Vector3D& albedo, float fuzziness):Albedo(albedo), Fuzziness(MathHelper::Clamp(fuzziness,0.0f,1.0f))
{
}

bool Metal::ScatterRay(const Ray& rayIn, const HitRecord& record, Vector3D& attenuation, Ray& rayScatter) const
{
    attenuation = Albedo;
    rayScatter.Origin = record.Position;
    rayScatter.Direction = ReflectedVector(rayIn.Direction, record.Normal) + Fuzziness * RandomUnitVector();

    return Dot(rayScatter.Direction, record.Normal) > 0;
}

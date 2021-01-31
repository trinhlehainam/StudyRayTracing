#include "Metal.h"

#include "../Common/Ray.h"
#include "../Common/MathHelper.h"
#include "../HitRecord.h"

Metal::Metal(const Vector3D& albedo, float fuzziness):Albedo(albedo), Fuzziness(MathHelper::Clamp(fuzziness,0.0f,1.0f))
{
}

bool Metal::ScatterRay(const Ray& inRay, const HitRecord& record, Vector3D& attenuation, Ray& scatteredRay) const
{
    attenuation = Albedo;
    scatteredRay.Origin = record.Position;
    scatteredRay.Time = inRay.Time;
    scatteredRay.Direction = ReflectedVector(inRay.Direction, record.Normal) + Fuzziness * RandomUnitVector();
    
    return Dot(scatteredRay.Direction, record.Normal) > 0;
}

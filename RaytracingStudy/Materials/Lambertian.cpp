#include "Lambertian.h"

#include "../Common/Ray.h"
#include "../HitRecord.h"

Lambertian::Lambertian(const Vector3D& albedo):Albedo(albedo)
{
}

bool Lambertian::ScatterRay(const Ray& inRay, const HitRecord& record, Vector3D& attenuation, Ray& scatteredRay) const
{
    attenuation = Albedo;
    scatteredRay.Origin = record.Position;
    scatteredRay.Time = inRay.Time;
    scatteredRay.Direction = record.Normal + RandomUnitVector();
    
    scatteredRay.Direction = scatteredRay.Direction.IsNearZero() ? record.Normal : scatteredRay.Direction;
    

    return true;
}

#include "Lambertian.h"

#include "../Common/Ray.h"
#include "../HitRecord.h"
#include "../Textures/SolidColor.h"

Lambertian::Lambertian(const Color3& albedo):Albedo(std::make_shared<SolidColor>(albedo))
{
}

Lambertian::Lambertian(std::shared_ptr<ITexture> texture):Albedo(texture)
{
}

bool Lambertian::ScatterRay(const Ray& inRay, const HitRecord& record, Color3& attenuation, Ray& scatteredRay) const
{
    attenuation = Albedo->GetColor(record.U,record.V,record.Normal);
    scatteredRay.Origin = record.Position;
    scatteredRay.Time = inRay.Time;
    scatteredRay.Direction = record.Normal + RandomUnitVector();
    
    scatteredRay.Direction = scatteredRay.Direction.IsNearZero() ? record.Normal : scatteredRay.Direction;
    
    return true;
}

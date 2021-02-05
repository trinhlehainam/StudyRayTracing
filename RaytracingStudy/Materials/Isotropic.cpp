#include "Isotropic.h"

#include "../Common/Ray.h"
#include "../Textures/SolidColor.h"
#include "../HitRecord.h"

Isotropic::Isotropic(const Color3& albedo) :Albedo(std::make_shared<SolidColor>(albedo))
{
}

bool Isotropic::ScatterRay(const Ray& inRay, const HitRecord& record, Vector3D& attenuation, Ray& scatteredRay) const
{
    attenuation = Albedo->GetColor(record.U, record.V, record.Position);
    scatteredRay.Direction = RandomPositionInUnitSphere();
    scatteredRay.Origin = record.Position;
    scatteredRay.Time = inRay.Time;
    return true;
}

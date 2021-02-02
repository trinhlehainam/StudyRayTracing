#include "DiffuseLight.h"

#include "../Textures/ITexture.h"
#include "../Textures/SolidColor.h"

DiffuseLight::DiffuseLight(const Color3& lightColor):Light(std::make_shared<SolidColor>(lightColor))
{
}

DiffuseLight::DiffuseLight(std::shared_ptr<ITexture> lightTex):Light(lightTex)
{
}

bool DiffuseLight::ScatterRay(const Ray& inRay, const HitRecord& record, Vector3D& attenuation, Ray& scatteredRay) const
{
    return false;
}

bool DiffuseLight::EmitRay(float u, float v, const Position3& pos, Color3& color) const
{
    color = Light->GetColor(u, v, pos);
    return true;
}

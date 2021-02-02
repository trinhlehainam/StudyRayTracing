#pragma once
#include "IMaterial.h"

#include <memory>

class ITexture;

class DiffuseLight :
    public IMaterial
{
public:
    explicit DiffuseLight(const Color3& lightColor);
    explicit DiffuseLight(std::shared_ptr<ITexture> lightTex);

    virtual bool ScatterRay(const Ray& inRay, const HitRecord& record, Vector3D& attenuation, Ray& scatteredRay) const override;
    virtual bool EmitRay(float u, float v, const Position3& pos, Color3& color) const override;

    std::shared_ptr<ITexture> Light;
};


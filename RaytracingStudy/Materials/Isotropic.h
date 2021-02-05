#pragma once
#include "IMaterial.h"

#include <memory>

class ITexture;

class Isotropic :
    public IMaterial
{
public:
    explicit Isotropic(const Color3& albedo);

    virtual bool ScatterRay(const Ray& inRay, const HitRecord& record, Vector3D& attenuation, Ray& scatteredRay) const override;

    std::shared_ptr<ITexture> Albedo;
};


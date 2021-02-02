#pragma once
#include "IMaterial.h"

#include <memory>

#include "../Textures/ITexture.h"

class Lambertian : public IMaterial
{
public:
	explicit Lambertian(const Color3& albedo);
	explicit Lambertian(std::shared_ptr<ITexture> texture);
	virtual bool ScatterRay(const Ray& inRay, const HitRecord& record, Color3& attenuation, Ray& scatteredRay) const override;

public:
	std::shared_ptr<ITexture> Albedo;
};


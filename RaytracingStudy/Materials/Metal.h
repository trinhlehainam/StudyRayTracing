#pragma once
#include "IMaterial.h"

#include "../Common/Vector3D.h"

class Metal :
    public IMaterial
{
public:
    explicit Metal(const Vector3D& albedo, float fuzziness);

    virtual bool ScatterRay(const Ray& inRay, const HitRecord& record, Vector3D& attenuation, Ray& scatteredRay) const override;

public:
    Vector3D Albedo;
    float Fuzziness;
};


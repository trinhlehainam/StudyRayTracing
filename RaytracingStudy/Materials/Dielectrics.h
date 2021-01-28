#pragma once
#include "IMaterial.h"

#include "../Common/Vector3D.h"

class Dielectrics :
    public IMaterial
{
public:
    Dielectrics(float refractionIndex);
    virtual bool ScatterRay(const Ray& inRay, const HitRecord& record, Vector3D& attenuation, Ray& scatteredRay) const override;

    float RefractionIndex;
};


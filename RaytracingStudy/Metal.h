#pragma once
#include "Material.h"

#include "Common/Vector3D.h"

class Metal :
    public Material
{
public:
    explicit Metal(const Vector3D& albedo, float fuzziness);

    virtual bool ScatterRay(const Ray& rayIn, const HitRecord& record, Vector3D& attenuation, Ray& rayScatter) const override;

public:
    Vector3D Albedo;
    float Fuzziness;
};


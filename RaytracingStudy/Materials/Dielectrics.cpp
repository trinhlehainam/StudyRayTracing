#include "Dielectrics.h"

#include <cmath>

#include "../Common/MathHelper.h"
#include "../Common/Ray.h"
#include "../HitRecord.h"

namespace
{
    // Schlick's approximation
    float Reflectance(float cosTheta, float refractionRatio)
    {
        float R0 = (1.0f - refractionRatio) / (1.0f + refractionRatio);
        R0 *= R0;
        return R0 + (1.0f - R0) * powf(1.0f - cosTheta, 5.0f);
    }
}

Dielectrics::Dielectrics(float refractionIndex):RefractionIndex(refractionIndex)
{
}

bool Dielectrics::ScatterRay(const Ray& inRay, const HitRecord& record, Vector3D& attenuation, Ray& scatteredRay) const
{
    attenuation = Vector3D(1.0f,1.0f,1.0f);
    scatteredRay.Origin = record.Position;

    Vector3D unitInVector = Normalize(inRay.Direction);
    float cos_theta = Dot(-unitInVector, record.Normal);
    float sin_theta = std::sqrtf(1.0f - cos_theta * cos_theta);
    float refactionRatio = record.FrontFace ? 1.0f / RefractionIndex : RefractionIndex;
    const bool refraction_failed = sin_theta * refactionRatio > 1.0f;

    scatteredRay.Direction =
        refraction_failed || Reflectance(cos_theta, refactionRatio) > MathHelper::Random<float>() ?
        ReflectedVector(unitInVector, record.Normal) :
        RefractedVector(unitInVector, record.Normal, refactionRatio);

    return true;
}

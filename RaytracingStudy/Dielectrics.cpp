#include "Dielectrics.h"

#include "Common/Ray.h"
#include "HitRecord.h"

Dielectrics::Dielectrics(float refractionIndex):RefractionIndex(refractionIndex)
{
}

bool Dielectrics::ScatterRay(const Ray& inRay, const HitRecord& record, Vector3D& attenuation, Ray& scatteredRay) const
{
    attenuation = Vector3D(1.0f,1.0f,1.0f);

    float refactionRatio = record.FrontFace ? 1.0f / RefractionIndex : RefractionIndex;
    scatteredRay.Origin = record.Position;
    scatteredRay.Direction = RefractedVector(Normalize(inRay.Direction), record.Normal, refactionRatio);

    return true;
}

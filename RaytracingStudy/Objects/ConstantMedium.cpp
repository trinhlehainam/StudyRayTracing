#include "ConstantMedium.h"

#include <iostream>

#include "../Common/MathHelper.h"
#include "../Common/Ray.h"
#include "../HitRecord.h"
#include "../AABB.h"
#include "../Materials/Isotropic.h"
#include "../Textures/ITexture.h"

ConstantMedium::ConstantMedium(std::shared_ptr<IHitable> boundary, std::shared_ptr<IMaterial> material,
    float density):Boundary(boundary), PhaseFunction(material), Inv_Neg_Density(-1.0f/density)
{
}

ConstantMedium::ConstantMedium(std::shared_ptr<IHitable> boundary, const Color3& color, float density):
    Boundary(boundary), PhaseFunction(std::make_shared<Isotropic>(color)), Inv_Neg_Density(-1.0f / density)
{
}

bool ConstantMedium::IsHit(const Ray& ray, float rangeMin, float rangeMax, HitRecord& record) const
{
    const bool enableDebug = false;
    const bool debugging = enableDebug && MathHelper::Random<float>() < 0.00001f;

    // Record when the ray first intersect with the volume
    // And record when the ray go through the volume and intersect other side of volume
    HitRecord record_min, record_max;

    //
    /* Checking volume by letting the LINE contents ray go through the object */
    //
    if (!Boundary->IsHit(ray, -MathHelper::INFINITY_FLOAT, MathHelper::INFINITY_FLOAT, record_min))
        return false;
    if (!Boundary->IsHit(ray, record_min.t + 0.001f, MathHelper::INFINITY_FLOAT, record_max))
        return false;
    /*------------------------------------------------------------------------*/

    if (debugging)
        std::cerr << "\n t_min = " << record_min.t << "\n t_max = " << record_max.t << "\n";

    record_min.t = record_min.t < rangeMin ? rangeMin : record_min.t;
    record_max.t = record_max.t > rangeMax ? rangeMax : record_max.t;

    if (record_min.t > record_max.t) return false;

    if (record_min.t < 0.0f) record_min.t = 0.0f;

    const float ray_length = ray.Direction.Length();
    const float travel_distance = (record_max.t - record_min.t) * ray_length;
    const float hit_distance = Inv_Neg_Density * std::logf(MathHelper::Random<float>());

    // Hit only when generated distance (hit_distance) is inside the volume
    if (hit_distance > travel_distance) return false;

    record.t = record_min.t + hit_distance / ray_length;
    record.Position = ray.GetPositionFromParameter(record.t);
    record.pMaterial = PhaseFunction;

    if (debugging)
        std::cerr << "\n hit_distance = " << hit_distance <<
        "\n record.t = " << record.t << "\n";

    // Abitrary, it doesn't effect the scattered ray from Isotropic Material
    record.Normal = Vector3D(1.0f, 0.0f, 0.0f);
    record.FrontFace = true;
    
    return true;
}

bool ConstantMedium::IsBoundingBox(AABB& output) const
{
    return Boundary->IsBoundingBox(output);
}

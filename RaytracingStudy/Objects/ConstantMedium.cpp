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

    HitRecord record_1, record_2;

    if (!Boundary->IsHit(ray, -MathHelper::INFINITY_FLOAT, MathHelper::INFINITY_FLOAT, record_1))
        return false;
    if (!Boundary->IsHit(ray, record_1.t + 0.001f, MathHelper::INFINITY_FLOAT, record_2))
        return false;

    if (debugging)
        std::cerr << "\n t_min = " << record_1.t << "\n t_max = " << record_2.t << "\n";

    record_1.t = MathHelper::Max(record_1.t, rangeMin);
    record_2.t = MathHelper::Min(record_2.t, rangeMax);

    if (record_1.t < 0.0f) record_1.t = 0.0f;

    const float ray_length = ray.Direction.Length();
    const float travel_distance = (record_2.t - record_1.t) * ray_length;
    const float hit_distance = Inv_Neg_Density * std::logf(MathHelper::Random<float>());

    if (hit_distance > travel_distance) return false;

    record.t = record_1.t + travel_distance / ray_length;
    record.Position = ray.GetPositionFromParameter(record.t);

    if (debugging)
        std::cerr << "\n hit_distance = " << hit_distance <<
        "\n record.t = " << record.t << "\n";

    record.Normal = Vector3D(1.0f, 0.0f, 0.0f);
    record.FrontFace = true;
    record.pMaterial = PhaseFunction;

    return true;
}

bool ConstantMedium::IsBoundingBox(AABB& output) const
{
    return Boundary->IsBoundingBox(output);
}

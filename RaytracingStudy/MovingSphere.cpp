#include "MovingSphere.h"

#include <cmath>

#include "HitRecord.h"

MovingSphere::MovingSphere(const Position3& center1, const Position3& center2, float radius,
    const std::shared_ptr<IMaterial>& material):
    Center1(center1), Center2(center2), Radius(radius), pMaterial(material)
{
}

bool MovingSphere::IsHit(const Ray& ray, float minRange, float maxRange, HitRecord& record) const
{
	Vector3D oc = ray.Origin - GetCenter(ray.Time);
	float a = Dot(ray.Direction, ray.Direction);
	float b = 2 * Dot(ray.Direction, oc);
	float c = Dot(oc, oc) - Radius * Radius;
	float d = b * b - 4 * a * c;

	if (d < 0) return false;

	float sqrt = std::sqrt(d);
	float root = (-b - sqrt) / (2 * a);
	if (root < minRange || root > maxRange)
	{
		root = (-b + sqrt) / (2 * a);
		if (root < minRange || root > maxRange)
			return false;
	}

	record.t = root;
	record.Position = ray.GetPositionFromParameter(root);
	Vector3D outwardNormal = (record.Position - GetCenter(ray.Time)) / Radius;
	record.SetFaceNormal(ray, outwardNormal);
	record.pMaterial = pMaterial;

	return true;
}

Position3 MovingSphere::GetCenter(float time) const
{
    return (1.0f - time) * Center1 + time * Center2;
}

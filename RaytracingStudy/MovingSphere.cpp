#include "MovingSphere.h"

#include <cmath>

#include "Common/MathHelper.h"
#include "HitRecord.h"
#include "AABB.h"

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
	GetSphereUV(record.Normal, record.U, record.V);
	record.pMaterial = pMaterial;

	return true;
}

bool MovingSphere::IsBoundingBox(AABB& output) const
{
	Vector3D offset = Vector3D(Radius, Radius, Radius);

	AABB a(Center1 - offset, Center1 + offset);
	AABB b(Center2 - offset, Center2 + offset);

	output = AABB::SurroundingBox(a, b);

	return true;
}

void MovingSphere::GetSphereUV(const Position3& point, float& u, float& v) const
{
	// Point is the position in surface of unit sphere
	// theta is angle from Y = -1 to Y = +1
	// phi is angle around Y axis start from X =-1 and return to this in one circle

	float theta = std::acosf(-point.Y);
	float phi = std::atan2f(-point.Z, point.X) + MathHelper::PI<float>;

	u = phi / (2.0f * MathHelper::PI<float>);
	v = theta / MathHelper::PI<float>;
}

Position3 MovingSphere::GetCenter(float time) const
{
    return (1.0f - time) * Center1 + time * Center2;
}

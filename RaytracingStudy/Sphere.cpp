#include "Sphere.h"

#include <cmath>

Sphere::Sphere():Center(0.0f,0.0,-1.0f),Radius(0.5f)
{
}

Sphere::Sphere(const Vector3D& center, float radius):Center(center),Radius(radius)
{
}

bool Sphere::CheckHit(const Ray& ray, float minRange, float maxRange, HitRecord& record) const
{
	Vector3D oc = ray.Origin - Center;
	float a = Dot(ray.Direction, ray.Direction);
	float b = 2 * Dot(ray.Direction, oc);
	float c = Dot(oc, oc) - Radius * Radius;
	float d = b * b - 4 * a * c;

	if (d > 0)
	{
		float length = (-b - std::sqrt(d)) / 2 * a;
		if (minRange <= length && length <= maxRange)
		{
			record.t = length;
			record.Position = ray.GetPositionFromParameter(length);
			record.Normal = Normalize(ray.GetPositionFromParameter(length) - Center);
			return true;
		}

		length = (-b + std::sqrt(d)) / 2 * a;
		if (minRange <= length && length <= maxRange)
		{
			record.t = length;
			record.Position = ray.GetPositionFromParameter(length);
			record.Normal = Normalize(ray.GetPositionFromParameter(length) - Center);
			return true;
		}
	}

    return false;
}

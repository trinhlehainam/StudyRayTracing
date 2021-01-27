#include "Sphere.h"

#include <cmath>

#include "HitRecord.h"

Sphere::Sphere():Center(0.0f,0.0,-1.0f),Radius(0.5f)
{
}

Sphere::Sphere(const Vector3D& center, float radius):Center(center),Radius(radius)
{
}

bool Sphere::IsHit(const Ray& ray, float minRange, float maxRange, HitRecord& record) const
{
	Vector3D oc = ray.Origin - Center;
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
	Vector3D outwardNormal = (record.Position - Center) / Radius;
	record.SetFaceNormal(ray, outwardNormal);
	
    return true;
}

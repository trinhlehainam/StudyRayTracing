#include "Ray.h"

Ray::Ray()
{
}

Ray::Ray(const Vector3D& origin, const Vector3D& point):Origin(origin), Direction(point - origin)
{
}

Vector3D Ray::GetPositionFromParameter(float t) const
{
	return Origin + Direction * t;
}

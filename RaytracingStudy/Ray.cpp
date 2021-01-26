#include "Ray.h"

Ray::Ray()
{
}

Ray::Ray(const Vector3D& origin, const Vector3D& direction):Origin(origin), Direction(direction)
{
}

Vector3D Ray::GetPositionFromParameter(float t) const
{
	return Origin + Direction * t;
}

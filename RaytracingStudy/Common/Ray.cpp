#include "Ray.h"

Ray::Ray():Time(0.0f)
{
}

Ray::Ray(const Vector3D& origin, const Vector3D& direction, float time):
	Origin(origin), Direction(direction), Time(time)
{
}

Vector3D Ray::GetPositionFromParameter(float t) const
{
	return Origin + t * Direction;
}

#include "RotateY.h"

#include <cmath>

#include "../Common/MathHelper.h"
#include "../Common/Ray.h"
#include "../HitRecord.h"

RotateY::RotateY(std::shared_ptr<IHitable> object, float angle):
	pObject(object),
	CosTheta(std::cosf(MathHelper::DegreeToRadian(angle))),
	SinTheta(std::sinf(MathHelper::DegreeToRadian(angle))),
	IsBB(pObject->IsBoundingBox(Box))
{
	if (!IsBB) return;
	Position3 max = Position3(-MathHelper::INFINITY_FLOAT, -MathHelper::INFINITY_FLOAT, -MathHelper::INFINITY_FLOAT);
	Position3 min = Position3(MathHelper::INFINITY_FLOAT, MathHelper::INFINITY_FLOAT, MathHelper::INFINITY_FLOAT);

	Position3 center = (Box.Max - Box.Min) / 2.0f;

	for (int i = 0; i < 2; ++i)
		for (int j = 0; j < 2; ++j)
			for (int k = 0; k < 2; ++k)
			{
				float x = (1 - i) * Box.Min.X + i * Box.Max.X;
				float y = (1 - j) * Box.Min.Y + j * Box.Max.Y;
				float z = (1 - k) * Box.Min.Z + k * Box.Max.Z;

				float new_x = CosTheta * x + SinTheta * z;
				float new_z = -SinTheta * x + CosTheta * z;

				Position3 test(new_x, y, new_z);

				min.X = MathHelper::Min(min.X, test.X);
				min.Y = MathHelper::Min(min.Y, test.Y);
				min.Z = MathHelper::Min(min.Z, test.Z);

				max.X = MathHelper::Max(max.X, test.X);
				max.Y = MathHelper::Max(max.Y, test.Y);
				max.Z = MathHelper::Max(max.Z, test.Z);
			}

	Box.Min = min;
	Box.Max = max;
}

bool RotateY::IsHit(const Ray& ray, float rangeMin, float rangeMax, HitRecord& record) const
{
	Position3 origin = ray.Origin;
	Vector3D dir = ray.Direction;

	origin.X = CosTheta * ray.Origin.X - SinTheta * ray.Origin.Z;
	origin.Z = SinTheta * ray.Origin.X + CosTheta * ray.Origin.Z;

	dir.X = CosTheta * ray.Direction.X - SinTheta * ray.Direction.Z;
	dir.Z = SinTheta * ray.Direction.X + CosTheta * ray.Direction.Z;

	Ray rotated_ray(origin, dir, ray.Time);
	if (!pObject->IsHit(rotated_ray, rangeMin, rangeMax, record)) return false;

	Position3 pos = record.Position;
	Vector3D normal = record.Normal;

	pos.X = CosTheta * record.Position.X + SinTheta * record.Position.Z;
	pos.Z = -SinTheta * record.Position.X + CosTheta * record.Position.Z;

	normal.X = CosTheta * record.Normal.X + SinTheta * record.Normal.Z;
	normal.Z = -SinTheta * record.Normal.X + CosTheta * record.Normal.Z;

	record.Position = pos;
	record.SetFaceNormal(rotated_ray, normal);

	return true;
}

bool RotateY::IsBoundingBox(AABB& output) const
{
	if (!IsBB) return false;
	output = Box;
	return true;
}

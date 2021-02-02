#pragma once

struct HitRecord;
struct Vector3D;
struct Ray;
using Color3 = Vector3D;
using Position3 = Vector3D;

class IMaterial
{
public:
	virtual ~IMaterial() {};

	virtual bool ScatterRay(const Ray& inRay, const HitRecord& record, Vector3D& attenuation, Ray& scatteredRay) const = 0;

	virtual bool EmitRay(float u, float v, const Position3& pos, Color3& color) const { return false; }
};


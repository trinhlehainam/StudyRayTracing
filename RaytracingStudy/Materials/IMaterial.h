#pragma once

struct HitRecord;
struct Vector3D;
struct Ray;

class IMaterial
{
public:
	virtual ~IMaterial() {};

	virtual bool ScatterRay(const Ray& inRay, const HitRecord& record, Vector3D& attenuation, Ray& scatteredRay) const = 0;

};


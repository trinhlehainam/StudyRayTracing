#pragma once

struct HitRecord;
struct Vector3D;
struct Ray;

class Material
{
public:
	virtual ~Material() {};

	virtual bool ScatterRay(const Ray& inRay, const HitRecord& record, Vector3D& attenuation, Ray& scatteredRay) const = 0;

};


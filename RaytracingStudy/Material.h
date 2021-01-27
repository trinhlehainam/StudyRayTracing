#pragma once

struct HitRecord;
struct Vector3D;
struct Ray;

class Material
{
public:
	virtual ~Material() {};

	virtual bool ScatterRay(const Ray& rayIn, const HitRecord& record, Vector3D& attenuation, Ray& rayScatter) const = 0;

};


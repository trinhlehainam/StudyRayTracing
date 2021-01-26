#pragma once
#include "Common/Ray.h"

struct HitRecord
{
	float t;
	Vector3D Position;
	Vector3D Normal;
	bool FrontFace;
	
	void SetFaceNormal(const Ray& ray, const Vector3D& outwardNomarl);
};


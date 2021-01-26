#include "HitRecord.h"

void HitRecord::SetFaceNormal(const Ray& ray, const Vector3D& outwardNomarl)
{
	FrontFace = Dot(ray.Direction, outwardNomarl) < 0;
	Normal = FrontFace ? outwardNomarl : -outwardNomarl;
}

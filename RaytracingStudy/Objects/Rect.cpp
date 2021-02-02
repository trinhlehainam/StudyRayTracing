#include "Rect.h"

#include "../HitRecord.h"
#include "../AABB.h"
#include "../Materials/IMaterial.h"

XY_Rect::XY_Rect(float x0, float x1, float y0, float y1, float k, std::shared_ptr<IMaterial> mat):
    X0(x0),X1(x1),Y0(y0),Y1(y1),K(k), pMaterial(mat)
{
}

bool XY_Rect::IsHit(const Ray& ray, float rangeMin, float rangeMax, HitRecord& record) const
{
    float t = (K - ray.Origin.Z) / ray.Direction.Z;
    if (t < rangeMin || t > rangeMax) return false;

    float x = ray.Origin.X + t * ray.Direction.X;
    float y = ray.Origin.Y + t * ray.Direction.Y;

    if (x < X0 || x > X1 || y < Y0 || y > Y1) return false;

    record.t = t;
    record.Position = ray.GetPositionFromParameter(t);
    record.U = (x - X0) / (X1 - X0);
    record.V = (y - Y0) / (Y1 - Y0);
    record.SetFaceNormal(ray, Vector3D(0.0f, 0.0f, 1.0f));
    record.pMaterial = pMaterial;

    return true;
}

bool XY_Rect::IsBoundingBox(AABB& output) const
{
    output.Min = Position3(X0, Y0, K - 0.0001f);
    output.Max = Position3(X1, Y1, K + 0.0001f);

    return true;
}



XZ_Rect::XZ_Rect(float x0, float x1, float z0, float z1, float k, std::shared_ptr<IMaterial> mat):
    X0(x0),X1(x1),Z0(z0),Z1(z1),K(k),pMaterial(mat)
{
}

bool XZ_Rect::IsHit(const Ray& ray, float rangeMin, float rangeMax, HitRecord& record) const
{
    float t = (K - ray.Origin.Y) / ray.Direction.Y;
    if (t < rangeMin || t > rangeMax) return false;

    float x = ray.Origin.X + t * ray.Direction.X;
    float z = ray.Origin.Z + t * ray.Direction.Z;

    if (x < X0 || x > X1 || z < Z0 || z > Z1) return false;

    record.t = t;
    record.Position = ray.GetPositionFromParameter(t);
    record.U = (x - X0) / (X1 - X0);
    record.V = (z - Z0) / (Z1 - Z0);
    record.SetFaceNormal(ray, Vector3D(0.0f, 1.0f, 0.0f));
    record.pMaterial = pMaterial;

    return true;
}

bool XZ_Rect::IsBoundingBox(AABB& output) const
{
    output.Min = Position3(X0, K - 0.0001f, Z0);
    output.Max = Position3(X1, K + 0.0001f, Z1);

    return true;
}


YZ_Rect::YZ_Rect(float y0, float y1, float z0, float z1, float k, std::shared_ptr<IMaterial> mat):
    Y0(y0),Y1(y1),Z0(z0),Z1(z1),K(k),pMaterial(mat)
{

}

bool YZ_Rect::IsHit(const Ray& ray, float rangeMin, float rangeMax, HitRecord& record) const
{
    float t = (K - ray.Origin.X) / ray.Direction.X;
    if (t < rangeMin || t > rangeMax) return false;

    float y = ray.Origin.Y + t * ray.Direction.Y;
    float z = ray.Origin.Z + t * ray.Direction.Z;

    if (y < Y0 || y > Y1 || z < Z0 || z > Z1) return false;

    record.t = t;
    record.Position = ray.GetPositionFromParameter(t);
    record.U = (y - Y0) / (Y1 - Y0);
    record.V = (z - Z0) / (Z1 - Z0);
    record.SetFaceNormal(ray, Vector3D(1.0f, 0.0f, 0.0f));
    record.pMaterial = pMaterial;

    return true;
}

bool YZ_Rect::IsBoundingBox(AABB& output) const
{
    output.Min = Position3(K - 0.0001f, Y0, Z0);
    output.Max = Position3(K + 0.0001f, Y1, Z1);

    return true;
}

#include "Camera.h"

#include <cmath>

#include "MathHelper.h"

Camera::Camera():Position(0.0f,0.0f,0.0f),ViewBase(-1.0f,1.0f,-1.0f),BaseU(2.0f,0.0f,0.0f),
BaseV(0.0f,-2.0f,0.0f)
{
}

Camera::Camera(float FOV, float aspectRatio)
{
    float focusLength = 1.0f;
    float half_h = focusLength * (std::tanf(MathHelper::DegreeToRadian(FOV/2.0f)));
    float half_w = aspectRatio * half_h;

    Position = {};
    ViewBase = Vector3D(-half_w, half_h, -focusLength);
    BaseU = Vector3D(2.0f * half_w, 0.0f, 0.0f);
    BaseV = Vector3D(0.0f, -2.0f * half_h, 0.0f);
}

Ray Camera::GetRayAtScreenUV(float u, float v) const
{
    return Ray(Position, ViewBase + u * BaseU + v * BaseV - Position);
}

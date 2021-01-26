#include "Camera.h"

Camera::Camera():Position(0.0f,0.0f,0.0f),ViewBase(-1.0f,1.0f,-1.0f),BaseU(2.0f,0.0f,0.0f),
BaseV(0.0f,-2.0f,0.0f)
{
}

Ray Camera::GetRayAtScreenUV(float u, float v) const
{
    return Ray(Position, ViewBase + u * BaseU + v * BaseV - Position);
}

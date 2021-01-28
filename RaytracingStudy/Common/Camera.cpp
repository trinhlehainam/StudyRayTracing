#include "Camera.h"

#include <cmath>

#include "MathHelper.h"

Camera::Camera():Position(0.0f,0.0f,0.0f),ViewPosition(-1.0f,1.0f,-1.0f),ViewU(2.0f,0.0f,0.0f),
ViewV(0.0f,-2.0f,0.0f), LenRadius(0.0f) ,U(-1.0f,0.0f,0.0f), V(0.0f,1.0f,0.0f), W(0.0f,0.0f,-1.0f)
{
}

Camera::Camera(Position3 lookFrom, Position3 lookAt, Vector3D vectorUp, float FOV, float aspectRatio,
    float aperture, float focusDistance)
    :Position(lookFrom),
    W(Normalize(lookAt - lookFrom)),
    U(Normalize(Cross(vectorUp, W))),
    V(Cross(W, U)),
    LenRadius(aperture / 2.0f)
{
   /*
   * View port
   */
    float half_h = focusDistance * (std::tanf(MathHelper::DegreeToRadian(FOV/2.0f)));
    float half_w = aspectRatio * half_h;

    // Viewport space
    ViewPosition = Position + half_w * U + half_h * V + focusDistance * W ;
    ViewU = -2.0f * half_w * U;
    ViewV = -2.0f * half_h * V;
}

Ray Camera::GetRayAtScreenUV(float u, float v) const
{
    Vector3D random_pos = LenRadius * RandomPositionInUnitCircle();
    // Map values from XY to UV
    Vector3D offset = random_pos.X * U + random_pos.Y * V;

    return Ray(Position + offset, ViewPosition + u * ViewU + v * ViewV - Position - offset);
}

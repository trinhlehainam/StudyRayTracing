#include "Camera.h"

#include <cmath>

#include "MathHelper.h"

Camera::Camera(Position3 lookFrom, Position3 lookAt, Vector3D vectorUp, float FOV, float aspectRatio,
    float aperture, float focusDistance)
    :Position(lookFrom),
    LenRadius(aperture / 2.0f),
    W(Normalize(lookAt - lookFrom)),
    U((Normalize(Cross(vectorUp, W)))),
    V((Cross(W, U)))
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
    Vector3D random_pos = LenRadius * RandomPositionInDisk();
    // Map values from XY to UV
    Vector3D offset = random_pos.X * U + random_pos.Y * V;

    return Ray(Position + offset,
        ViewPosition + u * ViewU + v * ViewV - Position - offset,
        MathHelper::Random<float>(0.0f,1.0f));
}

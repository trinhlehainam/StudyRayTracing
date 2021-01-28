#include "Camera.h"

#include <cmath>

#include "MathHelper.h"

Camera::Camera():Position(0.0f,0.0f,0.0f),ViewPosition(-1.0f,1.0f,-1.0f),ViewU(2.0f,0.0f,0.0f),
ViewV(0.0f,-2.0f,0.0f)
{
}

Camera::Camera(Position3 lookFrom, Position3 lookAt, Vector3D vectorUp, float FOV, float aspectRatio)
    :Position(lookFrom)
{
    // Camera space
    Vector3D w = Normalize(lookAt - lookFrom);
    Vector3D u = Normalize(Cross(vectorUp, w));
    Vector3D v = Cross(w, u);

   /*
   * View port
   */
    float half_h = (std::tanf(MathHelper::DegreeToRadian(FOV/2.0f)));
    float half_w = aspectRatio * half_h;

    // Viewport space
    ViewPosition = Position + half_w * u + half_h * v + w ;
    ViewU = -2.0f * half_w * u;
    ViewV = -2.0f * half_h * v;
}

Ray Camera::GetRayAtScreenUV(float u, float v) const
{
    return Ray(Position, ViewPosition + u * ViewU + v * ViewV - Position);
}

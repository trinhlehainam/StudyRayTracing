#include "SolidColor.h"

SolidColor::SolidColor(const Color3& color): Color(color)
{
}

Color3 SolidColor::GetColor(float u, float v, const Position3& pos) const
{
    return Color;
}

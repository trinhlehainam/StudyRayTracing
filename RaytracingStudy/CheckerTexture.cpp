#include "CheckerTexture.h"

#include <cmath>

#include "SolidColor.h"

CheckerTexture::CheckerTexture(const Color3& color1, const Color3& color2):
    Odd(std::make_shared<SolidColor>(color1)), Even(std::make_shared<SolidColor>(color2))
{
}

CheckerTexture::CheckerTexture(std::shared_ptr<ITexture> odd, std::shared_ptr<ITexture> even):
    Odd(odd), Even(even)
{
}

Color3 CheckerTexture::GetColor(float u, float v, const Position3& pos) const
{
    float sines = std::sinf(1.0f * pos.X) * std::sinf(1.0f * pos.Y) * std::sinf(1.0f * pos.Z);

    return sines < 0.0f ? Odd->GetColor(u,v,pos) : Even->GetColor(u,v,pos);
}

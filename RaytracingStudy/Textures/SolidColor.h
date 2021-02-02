#pragma once
#include "ITexture.h"

class SolidColor :
    public ITexture
{
public:
    explicit SolidColor(const Color3& color);

    virtual Color3 GetColor(float u, float v, const Position3& pos) const override;

    Color3 Color;
};


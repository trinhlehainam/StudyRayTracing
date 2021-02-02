#pragma once
#include "ITexture.h"

class ImageTexture :
    public ITexture
{
public:
    explicit ImageTexture(const wchar_t* fileName);
    ~ImageTexture();

    virtual Color3 GetColor(float u, float v, const Position3& pos) const override;

    int Handle;
    int Width, Height;
};


#include "ImageTexture.h"

#include <cassert>

#include <DxLib.h>

#include "../Common/MathHelper.h"

ImageTexture::ImageTexture(const wchar_t* fileName):
    Handle(DxLib::LoadSoftImage(fileName))
{
    DxLib::GetSoftImageSize(Handle, &Width, &Height);
}

ImageTexture::~ImageTexture()
{
    if (Handle >= 0)
        DxLib::DeleteSoftImage(Handle);
}

Color3 ImageTexture::GetColor(float u, float v, const Position3& pos) const
{
    Color3 color(1.0f, 0.0f, 0.0f);
    if (Handle < 0) return color;

    u = MathHelper::Clamp(u, 0.0f, 1.0f);
    v = 1.0f - MathHelper::Clamp(v, 0.0f, 1.0f);    // Convert v to image coordinate

    int x = MathHelper::Min(static_cast<int>(u * Width), Width - 1);
    int y = MathHelper::Min(static_cast<int>(v * Height), Height - 1);

    float a = 0.0f;
    DxLib::GetPixelSoftImageF(Handle, x, y, &color.X, &color.Y, &color.Z, &a);

    return color;
}

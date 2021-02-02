#pragma once
#include "ITexture.h"

#include<memory>

class CheckerTexture :
    public ITexture
{
public:
    explicit CheckerTexture(const Color3& color1, const Color3& color2);
    explicit CheckerTexture(std::shared_ptr<ITexture> odd, std::shared_ptr<ITexture> even);
    
    virtual Color3 GetColor(float u, float v, const Position3& pos) const override;

    std::shared_ptr<ITexture> Odd;
    std::shared_ptr<ITexture> Even;

};


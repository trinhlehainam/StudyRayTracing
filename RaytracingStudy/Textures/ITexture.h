#pragma once
#include "../Common/Vector3D.h"

class ITexture
{
public:
	virtual ~ITexture() {};
	virtual Color3 GetColor(float u, float v, const Position3& pos) const = 0;
};


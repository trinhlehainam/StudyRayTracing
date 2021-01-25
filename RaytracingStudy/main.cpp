#include <iostream>

#include <DxLib.h>

#include "Ray.h"

namespace
{
	constexpr int screen_width = 800;
	constexpr int screen_height = 600;
	constexpr int color_bits = 32;
}

void DrawCaroBackground(int width, int height, int numCaroX, int numCaroY,
	unsigned int color1, unsigned int color2)
{
	for (int y = height - 1; y >= 0; --y)
	{
		for (int x = 0; x < width; ++x)
		{
			int checkCaro = y / (height / numCaroX) % 2 != 0 ? 1 : 0;
			if (x / (width / numCaroY) % 2 == checkCaro)
				DrawPixel(x, y, color1);
			else
				DrawPixel(x, y, color2);
		}
	}
}

Vector3D GradianColor(const Ray& ray)
{
	auto dir = ray.Direction;
	float t = 0.5f * (dir.Y + 1.0f);
	return t * Vector3D(0.0f, 0.0f, 0.0f) + (1.0f - t) * Vector3D(1.0f, 1.0f, 1.0f);
}

int main()
{
	ChangeWindowMode(true);
	SetGraphMode(screen_width, screen_height, color_bits);
	SetMainWindowText(_T("�ȈՔł̃��C�g���ł���"));
	DxLib_Init();

	while (!ProcessMessage())
	{
		DrawCaroBackground(screen_width, screen_height, 5, 5, GetColor(255, 255, 255), GetColor(255, 0, 0));

		Vector3D screenPos = { -1.0f,1.0f,-1.0f };
		Vector3D u = { 2.0f,0.0f,0.0f };
		Vector3D v = { 0.0f,-2.0f,0.0f };
		Vector3D camPos = { 0.0f,0.0f,0.0f };

		for (int y = screen_height - 1; y >= 0; --y)
		{
			for (int x = 0; x < screen_width; ++x)
			{
				float lengthU = static_cast<float>(x) / screen_width;
				float lengthV = static_cast<float>(y) / screen_height;
				Ray ray(camPos, screenPos + lengthU * u + lengthV * v );
				auto color = GradianColor(ray);
				DrawPixel(x, y, GetColor(255*color.X, 255*color.Y, 255*color.Z));
			}
		}
	}

	DxLib_End();
	return 0;
}
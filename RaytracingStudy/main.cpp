#include <iostream>
#include <memory>

#include <DxLib.h>

#include "Common/MathHelper.h"
#include "Common/Camera.h"
#include "HitableList.h"
#include "HitRecord.h"
#include "Sphere.h"

namespace
{
	constexpr int screen_width = 400;
	constexpr int screen_height = 400;
	constexpr int color_bits = 32;
}

void DrawCaroBackground(int width, int height, int numCaroX, int numCaroY,
	unsigned int color1, unsigned int color2)
{
	for (int y = 0; y < screen_height; ++y)
	{
		for (int x = 0; x < screen_width; ++x)
		{
			int checkCaro = y / (height / numCaroX) % 2 != 0 ? 1 : 0;
			if (x / (width / numCaroY) % 2 == checkCaro)
				DrawPixel(x, y, color1);
			else
				DrawPixel(x, y, color2);
		}
	}
}

Vector3D RayColor(const Ray& ray, IHitable* world, int depth)
{
	// Guard infinite ray bouncing
	if (depth <= 0)
		return Vector3D(0.0f, 0.0f, 0.0f);

	HitRecord record;
	if (world->IsHit(ray, 0.001f, MathHelper::INFINITY_FLOAT, record))
	{
		Vector3D target = record.Position + record.Normal + RandomUnitVector();
		return 0.5f * RayColor(Ray(record.Position, target - record.Position), world, depth - 1);
	}

	auto dir = ray.Direction;
	float t = record.t;
	t = 0.5f * (dir.Y + 1.0f);
	return (1.0f - t) * Vector3D(1.0f, 1.0f, 1.0f) + t * Vector3D(0.5f, 0.7f, 1.0f);
}

unsigned int GetColor(const Vector3D& hdrColor)
{
	int r = 255 * MathHelper::Clamp(hdrColor.X, 0.0f, 1.0f);
	int g = 255 * MathHelper::Clamp(hdrColor.Y, 0.0f, 1.0f);
	int b = 255 * MathHelper::Clamp(hdrColor.Z, 0.0f, 1.0f);
	return DxLib::GetColor(r, g, b);
}

int main()
{
	ChangeWindowMode(true);
	SetGraphMode(screen_width, screen_height, color_bits);
	SetMainWindowText(_T("Study Ray tracing"));
	DxLib_Init();

	while (!ProcessMessage())
	{
		Camera camera;
		std::shared_ptr<HitableList> List = std::make_shared<HitableList>();
		List->List.push_back(std::make_shared<Sphere>());
		List->List.push_back(std::make_shared<Sphere>(Vector3D(0.0f, -100.5f, -1.0f), 100.0f));
		const int max_depth = 10; // number of ray bouncing
		const int sample_per_pixel = 10;
		for (int y = 0; y < screen_height; ++y)
		{
			for (int x = 0; x < screen_width; ++x)
			{
				Vector3D color;
				// Sampling per pixel
				for (int s = 0; s < sample_per_pixel; ++s)
				{
					float lengthU = static_cast<float>(x + MathHelper::Random<float>()) / (screen_width - 1);
					float lengthV = static_cast<float>(y + MathHelper::Random<float>()) / (screen_height - 1);
				
					color += RayColor(camera.GetRayAtScreenUV(lengthU, lengthV), List.get(), max_depth);
				}
				color /= sample_per_pixel;

				// Gamma correction
				// Power color by 1/2 -> mean square root of it
				color.Pow(0.5f);

				DrawPixel(x, y, GetColor(color));
			}
		}
	}

	DxLib_End();
	return 0;
}
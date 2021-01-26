#include <iostream>
#include <memory>

#include <DxLib.h>

#include "MathHelper.h"
#include "HitableList.h"
#include "HitRecord.h"
#include "Sphere.h"
#include "Camera.h"

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

Vector3D RandomPositionInUnitSphere()
{
	return RandomVector(1.0f,-1.0f);
}

Vector3D WriteColor(const Ray& ray, std::shared_ptr<IHitable> world, int depth)
{
	// Guard infinite ray bouncing
	if (depth <= 0)
		return Vector3D(0.0f, 0.0f, 0.0f);

	HitRecord record;
	if (world->CheckHit(ray, 0.0f, MathHelper::INFINITY_FLOAT, record))
	{
		Vector3D target = record.Position + record.Normal + RandomPositionInUnitSphere();
		return 0.5f * WriteColor(Ray(record.Position, target - record.Position),world, depth - 1);
	}

	auto dir = ray.Direction;
	float t = record.t;
	t = 0.5f * (dir.Y + 1.0f);
	return (1.0f - t) * Vector3D(1.0f, 1.0f, 1.0f) + t * Vector3D(0.0f, 1.0f, 1.0f);
}

int main()
{
	ChangeWindowMode(true);
	SetGraphMode(screen_width, screen_height, color_bits);
	SetMainWindowText(_T("ŠÈˆÕ”Å‚ÌƒŒƒCƒgƒŒ‚Å‚Á‚¹"));
	DxLib_Init();

	while (!ProcessMessage())
	{
		DrawCaroBackground(screen_width, screen_height, 5, 5, GetColor(255, 255, 255), GetColor(255, 0, 0));

		Camera camera;
		std::shared_ptr<HitableList> List = std::make_shared<HitableList>();
		List->List.push_back(std::make_shared<Sphere>());
		List->List.push_back(std::make_shared<Sphere>(Vector3D(0.0f, -100.5f, -1.0f), 100.0f));
		const int max_depth = 50; // number of ray bouncing
		for (int y = 0; y < screen_height; ++y)
		{
			for (int x = 0; x < screen_width; ++x)
			{
				Vector3D color;
				float lengthU = static_cast<float>(x) / screen_width;
				float lengthV = static_cast<float>(y) / screen_height;
				//for (int s = -2; s < 3; ++s)
				//{
				//	int clamp = MathHelper::Clamp(s + x, 0, screen_width);
				//	float lengthU = static_cast<float>(clamp) / screen_width;
				//	color += WriteColor(camera.GetRayAtScreenUV(lengthU, lengthV), objectList, depth);
				//}
				//color /= 5;

				color += WriteColor(camera.GetRayAtScreenUV(lengthU, lengthV), List, max_depth);
				
				DrawPixel(x, y, GetColor(255*color.X, 255*color.Y, 255*color.Z));
			}
		}
	}

	DxLib_End();
	return 0;
}
#include <iostream>

#include <DxLib.h>

#include "MathHelper.h"
#include "HitableList.h"
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
	Vector3D p;
	do
	{
		float x = MathHelper::Random(-1.0f,1.0f);
		float y = MathHelper::Random(-1.0f,1.0f);
		float z = MathHelper::Random(-1.0f,1.0f);
		p = Vector3D(x, y, z);
	} while (Dot(p, p) >= 1);
	return p;
}

Vector3D Color(const Ray& ray, IHitable* world)
{
	HitRecord record;
	if (world->CheckHit(ray, 0.0f, 200.0f, record))
	{
		Vector3D target = record.Position + record.Normal + RandomPositionInUnitSphere();
		return 0.5f* Color(Ray(record.Position, target),world);
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
		IHitable* list[2];
		list[0] = new Sphere();
		list[1] = new Sphere(Vector3D(0.0f, -100.5f, -1.0f), 100.0f);
		IHitable* objectList = new HitableList(list, 2);
		for (int y = 0; y < screen_height; ++y)
		{
			for (int x = 0; x < screen_width; ++x)
			{
				Vector3D color;
				float lengthV = static_cast<float>(y) / screen_height;
				for (int s = -2; s < 3; ++s)
				{
					int clamp = MathHelper::Clamp(s + x, 0, screen_width);
					float lengthU = static_cast<float>(clamp) / screen_width;
					color += Color(camera.GetRayAtScreenUV(lengthU, lengthV), objectList);
				}
				color /= 5;
				
				DrawPixel(x, y, GetColor(255*color.X, 255*color.Y, 255*color.Z));
			}
		}
		delete objectList;
	}

	DxLib_End();
	return 0;
}
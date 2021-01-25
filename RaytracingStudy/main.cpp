#include <iostream>

#include <DxLib.h>

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

float IsHitObject(const Vector3D& sphereCenter, float radius, const Ray& ray)
{
	// Vector from view to center of sphere
	Vector3D oc = ray.Origin - sphereCenter;
	float a = Dot(ray.Direction, ray.Direction);
	float b = 2 * Dot(ray.Direction, oc);
	float c = Dot(oc, oc) - radius * radius;
	float d = b * b - 4 * a * c;
		
	if (d < 0)
		return -1;
	else
		return (-b - sqrt(d)) / (2 * a);
}

Vector3D Color(const Ray& ray, IHitable* object)
{
	HitRecord record;
	if (object->CheckHit(ray, 0.0f, 100.0f, record))
	{
		return 0.5f*(record.Normal + Vector3D(1.0f,1.0f,1.0f));
	}
	auto dir = ray.Direction;
	float t = record.t;
	t = 0.5f * (dir.Y + 1.0f);
	return t * Vector3D(0.0f, 0.0f, 0.0f) + (1.0f - t) * Vector3D(1.0f, 1.0f, 1.0f);
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

		Vector3D screenPos = { -1.0f,1.0f,-1.0f };
		Vector3D u = { 2.0f,0.0f,0.0f };
		Vector3D v = { 0.0f,-2.0f,0.0f };
		Vector3D camPos = { 0.0f,0.0f,0.0f };
		IHitable* sphere = new Sphere();
		for (int y = screen_height - 1; y >= 0; --y)
		{
			for (int x = 0; x < screen_width; ++x)
			{
				float lengthU = static_cast<float>(x) / screen_width;
				float lengthV = static_cast<float>(y) / screen_height;
				Ray ray(camPos, screenPos + lengthU * u + lengthV * v );
				auto color = Color(ray, sphere);
				DrawPixel(x, y, GetColor(255*color.X, 255*color.Y, 255*color.Z));
			}
		}
		delete sphere;
	}

	DxLib_End();
	return 0;
}
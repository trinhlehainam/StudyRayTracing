#include <iostream>
#include <memory>
#include <thread>

#include <DxLib.h>

#include "Common/MathHelper.h"
#include "Common/Camera.h"
#include "HitableList.h"
#include "HitRecord.h"
#include "Sphere.h"
#include "MovingSphere.h"
#include "BVHNode.h"
#include "Materials/Lambertian.h"
#include "Materials/Metal.h"
#include "Materials/Dielectrics.h"

namespace
{
	constexpr int screen_width = 800;
	constexpr int screen_height = 600;
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

Vector3D RayColor(const Ray& ray, const HitableList& world, int numRayBounce)
{
	// Guard infinite ray bouncing
	if (numRayBounce <= 0)
		return Vector3D(0.0f, 0.0f, 0.0f);

	HitRecord record;
	if (world.IsHit(ray, 0.001f, MathHelper::INFINITY_FLOAT, record))
	{
		Ray scatteredRay;
		Vector3D attenuation;
		if (record.pMaterial->ScatterRay(ray, record, attenuation, scatteredRay))
			return attenuation * RayColor(scatteredRay, world, numRayBounce - 1);
		return attenuation;
	}

	auto dir = Normalize(ray.Direction);
	float t = record.t;
	t = 0.5f * (dir.Y + 1.0f);
	return (1.0f - t) * Vector3D(1.0f, 1.0f, 1.0f) + t * Vector3D(0.5f, 0.7f, 1.0f);
}

unsigned int GetColor(const Vector3D& hdrColor)
{
	int r = static_cast<int>(255.0f * MathHelper::Clamp(hdrColor.X, 0.0f, 1.0f));
	int g = static_cast<int>(255.0f * MathHelper::Clamp(hdrColor.Y, 0.0f, 1.0f));
	int b = static_cast<int>(255.0f * MathHelper::Clamp(hdrColor.Z, 0.0f, 1.0f));
	return DxLib::GetColor(r, g, b);
}

HitableList RandomScene()
{
	HitableList world;
	auto ground_material = std::make_shared<Lambertian>(Color3(0.5f, 0.5f, 0.5f));
	world.Objects.push_back(std::make_shared<Sphere>(Position3(0.0f, -1000.0f, 0.0f), 1000.0f, ground_material));
	
	for (int a = -6; a < 6; ++a) {
		for (int b = -6; b < 6; ++b) {
			auto choose_mat = MathHelper::Random<float>();
			Position3 center(a + 0.9f * MathHelper::Random<float>(), 0.2f, b + 0.9f * MathHelper::Random<float>());
			if ((center - Position3(4.0f, 0.2f, 0.0f)).Length() > 0.9f) {
				std::shared_ptr<IMaterial> sphere_material;
				if (choose_mat < 0.8f) {
					// diffuse
					auto albedo = RandomVector(0.0f,1.0f) * RandomVector(0.0f, 1.0f);
					sphere_material = std::make_shared<Lambertian>(albedo);
					Position3 center2 = center + Vector3D(0.0f, MathHelper::Random<float>(0.0f, 0.5f), 0.0f);
					world.Objects.push_back(std::make_shared<MovingSphere>(center, center2, 0.2f, sphere_material));
				}
				else if (choose_mat < 0.95) {
					// metal
					auto albedo = RandomVector(0.5f, 1.0f);
					auto fuzz = MathHelper::Random<float>(0.0f, 0.5f);
					sphere_material = std::make_shared<Metal>(albedo, fuzz);
					world.Objects.push_back(std::make_shared<Sphere>(center, 0.2f, sphere_material));
				}
				else {
					// glass
					sphere_material = std::make_shared<Dielectrics>(1.5f);
					world.Objects.push_back(std::make_shared<Sphere>(center, 0.2f, sphere_material));
				}
			}
		}
	}
	
	auto material1 = std::make_shared<Dielectrics>(1.5f);
	world.Objects.push_back(std::make_shared<Sphere>(Position3(0.0f, 1.0f, 0.0f), 1.0f, material1));
	auto material2 = std::make_shared<Lambertian>(Color3(0.4f, 0.2f, 0.1f));
	world.Objects.push_back(std::make_shared<Sphere>(Position3(-4.0f, 1.0f, 0.0f), 1.0f, material2));
	auto material3 = std::make_shared<Metal>(Color3(0.7f, 0.6f, 0.5f), 0.0f);
	world.Objects.push_back(std::make_shared<Sphere>(Position3(4.0f, 1.0f, 0.0f), 1.0f, material3));

	return HitableList(std::make_shared<BVHNode>(world));
}


int main()
{
	ChangeWindowMode(true);
	SetGraphMode(screen_width, screen_height, color_bits);
	SetMainWindowText(_T("Cyberpunk 2077 REEL PARFORMANZ :)"));
	DxLib_Init();

	const Position3 look_from(13.0f, 2.0f, 3.0f);
	const Position3 look_at(0.0f, 0.0f, 0.0f);
	const Vector3D up(0.0f, 1.0f, 0.0f);
	constexpr float aspect_ratio = static_cast<float>(screen_width) / screen_height;
	constexpr float aperture = 0.1f;
	constexpr float focus_distance = 10.0f;
	Camera camera(
		look_from,
		look_at,
		up,
		20.0f,
		aspect_ratio,
		aperture,
		focus_distance);

	constexpr int max_bounce = 5;								
	constexpr int sample_per_pixel = 50;

	HitableList World = RandomScene();

	while (!ProcessMessage())
	{
		int time = GetNowCount();

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
				
					color += RayColor(camera.GetRayAtScreenUV(lengthU, lengthV), World, max_bounce);
				}
				color /= sample_per_pixel;

				// Gamma correction
				// Power color by 1/2 -> mean square root of it
				color.Pow(0.5f);

				DrawPixel(x, y, GetColor(color));
			}
		}
		float deltaTime = (GetNowCount() - time) / 1000.0f;
		DxLib::DrawFormatString(10, 10, DxLib::GetColor(0, 0, 0), L"Elapsed time : %f seconds", deltaTime);
	}

	DxLib_End();
	return 0;
}
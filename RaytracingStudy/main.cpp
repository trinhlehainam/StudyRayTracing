#include <iostream>
#include <memory>
#include <thread>

#include <DxLib.h>

#include "Common/MathHelper.h"
#include "Common/Camera.h"
#include "Objects/HitableList.h"
#include "HitRecord.h"
#include "Objects/Sphere.h"
#include "Objects/MovingSphere.h"
#include "Objects/BVHNode.h"
#include "Objects/Rect.h"
#include "Materials/Lambertian.h"
#include "Materials/Metal.h"
#include "Materials/Dielectrics.h"
#include "Materials/DiffuseLight.h"
#include "Textures/CheckerTexture.h"
#include "Textures/ImageTexture.h"

namespace
{
	constexpr int screen_width = 800;
	constexpr int screen_height = 600;
	constexpr int color_bits = 32;
}

void DrawCheckerBoard(int width, int height, int numCaroX, int numCaroY,
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

Color3 RayColor(const Ray& ray, const Color3& background, const HitableList& world, int numBounce)
{
	// If ray reach bounce limit, the light in ray is amlost absorded
	if (numBounce <= 0)
		return Color3(0.0f, 0.0f, 0.0f);

	// If ray doesn't hit any objects, return background's color
	HitRecord record;
	if (!world.IsHit(ray, 0.0001f, MathHelper::INFINITY_FLOAT, record))
	{
		return background;
	}

	Color3 light;
	record.pMaterial->EmittedRay(record.U, record.V, record.Normal, light);

	Ray scatteredRay;
	// reflective of object (how much light will be reflected back when hit an object)
	Color3 attenuation;
	// If object's material doesn't reflect any light (or absorb all the light)
	if (!record.pMaterial->ScatterRay(ray, record, attenuation, scatteredRay))
		return light;

	return light + attenuation * RayColor(scatteredRay, background, world, numBounce - 1);
}

unsigned int GetColor(const Color3& hdrColor)
{
	int r = static_cast<int>(255.0f * MathHelper::Clamp(hdrColor.X, 0.0f, 1.0f));
	int g = static_cast<int>(255.0f * MathHelper::Clamp(hdrColor.Y, 0.0f, 1.0f));
	int b = static_cast<int>(255.0f * MathHelper::Clamp(hdrColor.Z, 0.0f, 1.0f));
	return DxLib::GetColor(r, g, b);
}

unsigned int GetColor(Color3& hdrColor, const int& samplesCount)
{
	hdrColor.X = hdrColor.X != hdrColor.X ? 0.0f : hdrColor.X;
	hdrColor.Y = hdrColor.Y != hdrColor.Y ? 0.0f : hdrColor.Y;
	hdrColor.Z = hdrColor.Z != hdrColor.Z ? 0.0f : hdrColor.Z;

	hdrColor /= static_cast<float>(samplesCount);

	// Gamma correction
	// Power color by 1/2 -> mean square root of it
	hdrColor.Pow(0.5f);

	int r = static_cast<int>(255.0f * MathHelper::Clamp(hdrColor.X, 0.0f, 1.0f));
	int g = static_cast<int>(255.0f * MathHelper::Clamp(hdrColor.Y, 0.0f, 1.0f));
	int b = static_cast<int>(255.0f * MathHelper::Clamp(hdrColor.Z, 0.0f, 1.0f));
	return DxLib::GetColor(r, g, b);
}

HitableList RandomScene()
{
	auto earthTex = std::make_shared<ImageTexture>(L"Resource/Texture/earth.png");
	auto rockTex = std::make_shared<ImageTexture>(L"Resource/Texture/rock.jpg");
	auto checkerTex = std::make_shared<CheckerTexture>(Color3(0.2f, 0.3f, 0.1f), Color3(0.9f, 0.9f, 0.9f));
	HitableList world;
	world.Objects.push_back(
		std::make_shared<Sphere>(Position3(0.0f, -1000.0f, 0.0f), 1000.0f, 
			std::make_shared<Lambertian>(checkerTex)));
	
	for (int a = -6; a < 6; ++a) {
		for (int b = -6; b < 6; ++b) {
			auto choose_mat = MathHelper::Random<float>();
			Position3 center(a + 0.9f * MathHelper::Random<float>(), 0.2f, b + 0.9f * MathHelper::Random<float>());
			if ((center - Position3(4.0f, 0.2f, 0.0f)).Length() > 0.9f) {
				std::shared_ptr<IMaterial> sphere_material;
				if (choose_mat < 0.8f) {
					// diffuse
					auto albedo = MathHelper::Random<float>(0.0f,2.0f) > 1.0f ? rockTex : earthTex;
					sphere_material = std::make_shared<Lambertian>(albedo);
					Position3 center2 = center + Vector3D(0.0f, MathHelper::Random<float>(0.0f, 0.5f), 0.0f);
					world.Objects.push_back(std::make_shared<Sphere>(center, 0.2f, sphere_material));
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
	auto material2 = std::make_shared<Lambertian>(earthTex);
	world.Objects.push_back(std::make_shared<Sphere>(Position3(-4.0f, 1.0f, 0.0f), 1.0f, material2));
	auto material3 = std::make_shared<Metal>(Color3(0.7f, 0.6f, 0.5f), 0.0f);
	world.Objects.push_back(std::make_shared<Sphere>(Position3(4.0f, 1.0f, 0.0f), 1.0f, material3));

	return HitableList(std::make_shared<BVHNode>(world));
}

HitableList LightScene()
{
	auto checkerTex = std::make_shared<CheckerTexture>(Color3(0.5f, 0.5f, 0.0f), Color3(1.0f, 1.0f, 1.0f));
	auto earthTex = std::make_shared<ImageTexture>(L"Resource/Texture/earth.png");

	HitableList world;
	auto material1 = std::make_shared<Lambertian>(checkerTex);
	auto material2 = std::make_shared<Lambertian>(earthTex);
	world.Objects.push_back(std::make_shared<Sphere>(Position3(0.0f, -1000.0f, 0.0f), 1000.0f, material1));
	world.Objects.push_back(std::make_shared<Sphere>(Position3(0.0f, 2.0f, 0.0f), 2.0f, material2));

	auto light = std::make_shared<DiffuseLight>(Color3(4.0f, 4.0f, 4.0f));
	world.Objects.push_back(std::make_shared<XY_Rect>(3.0f, 5.0f, 1.0f, 3.0f, -2.0f, light));
	world.Objects.push_back(std::make_shared<Sphere>(Position3(0.0f, 7.0f, 0.0f), 2.0f, light));

	return HitableList(std::make_shared<BVHNode>(world));
}

HitableList CornellBoxScene()
{
	HitableList world;

	auto red = std::make_shared<Lambertian>(Color3(0.65f, 0.05f, 0.05f));
	auto white = std::make_shared<Lambertian>(Color3(0.73f, 0.73f, 0.73f));
	auto green = std::make_shared<Lambertian>(Color3(0.12f, 0.45f, 0.15f));
	auto light = std::make_shared<DiffuseLight>(Color3(15.0f, 15.0f, 15.0f));
	auto metal = std::make_shared<Metal>(Color3(0.73f, 0.73f, 0.73f));

	world.Objects.push_back(std::make_shared<YZ_Rect>(0.0f, 555.0f, 0.0f, 555.0f, 555.0f, green));	 // left
	world.Objects.push_back(std::make_shared<YZ_Rect>(0.0f, 555.0f, 0.0f, 555.0f, 0.0f, red));		 // right
	world.Objects.push_back(std::make_shared<XZ_Rect>(0.0f, 555.0f, 0.0f, 555.0f, 555.0f, white));	 // top
	world.Objects.push_back(std::make_shared<XZ_Rect>(0.0f, 555.0f, 0.0f, 555.0f, 0.0f, white));	 // bottom
	world.Objects.push_back(std::make_shared<XY_Rect>(0.0f, 555.0f, 0.0f, 555.0f, 555.0f, white));   // back

	world.Objects.push_back(std::make_shared<XZ_Rect>(213.0f, 343.0f, 227.0f, 332.0f, 554.0f, light));

	return world;
}

HitableList TestMaterialsScene()
{
	HitableList world;
	auto ground = std::make_shared<Lambertian>(Color3(0.3f, 0.5f, 0.2f));
	auto lambertian = std::make_shared<Lambertian>(Color3(0.5f, 0.2f, 0.65f));
	auto metal = std::make_shared<Metal>(Color3(0.73f, 0.73f, 0.73f));
	auto dieletrics = std::make_shared<Dielectrics>(1.5f);
	auto light = std::make_shared<DiffuseLight>(Color3(30.0f, 30.0f, 30.0f));

	world.Add(std::make_shared<XZ_Rect>(-50.0f, 50.0f, -50.0f, 50.0f, 200.0f, light));
	world.Add(std::make_shared<XZ_Rect>(-500.0f, 500.0f, -500.0f, 500.0f, 0.0f, lambertian));
	//world.Add(std::make_shared<Sphere>(Position3(0.0f, -1000.0f, 0.0f), 1000.0f, lambertian));
	//world.Add(std::make_shared<Sphere>(Position3(0.0f, 0.5f, 0.0f), 0.5f, dieletrics));
	//world.Add(std::make_shared<Sphere>(Position3(1.0f, 0.5f, 0.0f), 0.5f, metal));
	//world.Add(std::make_shared<Sphere>(Position3(-1.0f, 0.5f, 0.0f), 0.5f, ground));

	return world;
}

int main()
{
	ChangeWindowMode(true);
	SetGraphMode(screen_width, screen_height, color_bits);
	SetMainWindowText(_T("Cyberpunk 2077 REEL PARFORMANZ :)"));
	DxLib_Init();

	HitableList World;
	Color3 background;
	Position3 look_from;
	Position3 look_at;
	Vector3D up(0.0f, 1.0f, 0.0f);
	constexpr float aspect_ratio = static_cast<float>(screen_width) / screen_height;
	float fov = 20.0f;
	float aperture = 0.0f;
	float focus_distance = 10.0f;
	int max_bounce = 10;
	int samples_per_pixel = 5;
	Color3 text;

	switch (3)
	{
	case 0:
		background = { 0.7f,0.8f,1.0f };
		look_from = { 13.0f, 2.0f, 3.0f };
		look_at = { 0.0f, 0.0f, 0.0f };
		aperture = 0.1f;
		max_bounce = 10;
		samples_per_pixel = 50;
		focus_distance = 10.0f;
		World = RandomScene();
		text = { 0.0f,0.0f,0.0f };
		break;
	case 1:
		background = { 0.0f,0.0f,0.0f };
		look_from = { 26.0f, 3.0f, 6.0f };
		look_at = { 0.0f, 2.0f, 0.0f };
		aperture = 0.1f;
		focus_distance = 10.0f;
		max_bounce = 10;
		samples_per_pixel = 400;
		text = { 1.0f,1.0f,1.0f };
		World = LightScene();
		break;
	case 2:
		background = { 0.0f,0.0f,0.0f };
		look_from = { 278.0f, 278.0f, -800.0f };
		look_at = { 278.0f, 278.0f, 0.0f };
		max_bounce = 50;
		samples_per_pixel = 200;
		fov = 40.0f;
		text = { 1.0f,1.0f,1.0f };
		World = CornellBoxScene();
		break;
	case 3:
		background = { 0.7f,0.8f,1.0f };
		look_from = { 1000.0f, 250.0f, 0.0f };
		look_at = { 0.0f, 250.0f, 0.0f };
		max_bounce = 10;
		samples_per_pixel = 100;
		fov = 40.0f;
		text = { 1.0f,1.0f,1.0f };
		World = TestMaterialsScene();
	}

	Camera camera(
		look_from,
		look_at,
		up,
		fov,
		aspect_ratio,
		aperture,
		focus_distance);

	while (!ProcessMessage())
	{
		int time = GetNowCount();

		for (int y = 0; y < screen_height; ++y)
		{
			for (int x = 0; x < screen_width; ++x)
			{
				Vector3D color;
				// Sampling per pixel
				for (int s = 0; s < samples_per_pixel; ++s)
				{
					float lengthU = static_cast<float>(x + MathHelper::Random<float>(-0.5f,0.5f)) / (screen_width - 1);
					float lengthV = static_cast<float>(y + MathHelper::Random<float>(-0.5f, 0.5f)) / (screen_height - 1);
				
					color += RayColor(camera.GetRayAtScreenUV(lengthU, lengthV), background, World, max_bounce);
				}

				DrawPixel(x, y, GetColor(color, samples_per_pixel));
			}
		}
		float deltaTime = (GetNowCount() - time) / 1000.0f;
		DxLib::DrawFormatString(10, 10, GetColor(text), L"Elapsed time : %f seconds", deltaTime);
	}

	DxLib_End();
	return 0;
}
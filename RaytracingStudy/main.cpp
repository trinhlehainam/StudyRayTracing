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
#include "Objects/Box.h"
#include "Objects/ConstantMedium.h"
#include "Transform/Translate.h"
#include "Transform/RotateY.h"
#include "Materials/Lambertian.h"
#include "Materials/Metal.h"
#include "Materials/Dielectrics.h"
#include "Materials/DiffuseLight.h"
#include "Materials/Isotropic.h"
#include "Textures/CheckerTexture.h"
#include "Textures/ImageTexture.h"
#include "Textures/SolidColor.h"

namespace
{
	constexpr int screen_width = 800;
	constexpr int screen_height = 600;
	constexpr int color_bits = 32;

	int select_index = 0;
	constexpr int num_scene = 6;

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
	Camera camera;

	using Scene_t = void(*)(void);
	Scene_t scene;
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
	if (!world.IsHit(ray, 0.001f, MathHelper::INFINITY_FLOAT, record))
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
	std::shared_ptr<ITexture> earthTex = std::make_shared<ImageTexture>(L"Resource/Texture/earth.png");
	std::shared_ptr<ITexture> rockTex = std::make_shared<ImageTexture>(L"Resource/Texture/rock.jpg");
	std::shared_ptr<ITexture> checkerTex = std::make_shared<CheckerTexture>(Color3(0.2f, 0.3f, 0.1f), Color3(0.9f, 0.9f, 0.9f));
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
					std::shared_ptr<ITexture> albedo = 
						std::make_shared<SolidColor>(RandomVector(0.1f, 1.0f));
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

	std::shared_ptr<IHitable> box1 = 
		std::make_shared<Box>(Position3(0.0f, 0.0f, 0.0f), Position3(165.0f, 330.0f, 165.0f), white);
	box1 = std::make_shared<RotateY>(box1, 15.0f);
	box1 = std::make_shared<Translate>(box1, Vector3D(265.0f, 0.0f, 295.0f));
	world.Add(box1);

	std::shared_ptr<IHitable> box2 = 
		std::make_shared<Box>(Position3(0.0f, 0.0f, 0.0f), Position3(165.0f, 165.0f, 165.0f), white);
	box2 = std::make_shared<RotateY>(box2, -18.0f);
	box2 = std::make_shared<Translate>(box2, Vector3D(130.0f, 0.0f, 65.0f));
	world.Add(box2);

	world.Objects.push_back(std::make_shared<XZ_Rect>(213.0f, 343.0f, 227.0f, 332.0f, 554.0f, light));

	return world;
}

HitableList CornellBoxVolumetricScene()
{
	HitableList world;

	auto red = std::make_shared<Lambertian>(Color3(0.65f, 0.05f, 0.05f));
	auto white = std::make_shared<Lambertian>(Color3(0.73f, 0.73f, 0.73f));
	auto green = std::make_shared<Lambertian>(Color3(0.12f, 0.45f, 0.15f));
	auto light = std::make_shared<DiffuseLight>(Color3(7.0f, 7.0f, 7.0f));
	auto metal = std::make_shared<Metal>(Color3(0.73f, 0.73f, 0.73f));

	world.Objects.push_back(std::make_shared<YZ_Rect>(0.0f, 555.0f, 0.0f, 555.0f, 555.0f, green));	 // left
	world.Objects.push_back(std::make_shared<YZ_Rect>(0.0f, 555.0f, 0.0f, 555.0f, 0.0f, red));		 // right
	world.Objects.push_back(std::make_shared<XZ_Rect>(0.0f, 555.0f, 0.0f, 555.0f, 555.0f, white));	 // top
	world.Objects.push_back(std::make_shared<XZ_Rect>(0.0f, 555.0f, 0.0f, 555.0f, 0.0f, white));	 // bottom
	world.Objects.push_back(std::make_shared<XY_Rect>(0.0f, 555.0f, 0.0f, 555.0f, 555.0f, white));   // back

	std::shared_ptr<IHitable> box1 =
		std::make_shared<Box>(Position3(0.0f, 0.0f, 0.0f), Position3(165.0f, 330.0f, 165.0f), white);
	box1 = std::make_shared<RotateY>(box1, 15.0f);
	box1 = std::make_shared<Translate>(box1, Vector3D(265.0f, 0.0f, 295.0f));

	std::shared_ptr<IHitable> box2 =
		std::make_shared<Box>(Position3(0.0f, 0.0f, 0.0f), Position3(165.0f, 165.0f, 165.0f), white);
	box2 = std::make_shared<RotateY>(box2, -18.0f);
	box2 = std::make_shared<Translate>(box2, Vector3D(130.0f, 0.0f, 65.0f));

	world.Add(std::make_shared<ConstantMedium>(box1, Color3(0.0f, 0.0f, 0.0f), 0.01f));
	world.Add(std::make_shared<ConstantMedium>(box2, Color3(1.0f, 1.0f, 1.0f), 0.01f));

	world.Objects.push_back(std::make_shared<XZ_Rect>(113.0f, 443.0f, 127.0f, 432.0f, 554.0f, light));

	return world;
}

HitableList TestMaterialsScene()
{
	HitableList world;
	auto ground = std::make_shared<Lambertian>(Color3(0.3f, 0.5f, 0.2f));
	auto lambertian = std::make_shared<Lambertian>(Color3(0.5f, 0.2f, 0.65f));
	auto metal = std::make_shared<Metal>(Color3(0.73f, 0.73f, 0.73f));
	auto dieletrics = std::make_shared<Dielectrics>(1.5f);
	auto light = std::make_shared<DiffuseLight>(Color3(30.0f, 30.0f, 0.0f));

	world.Add(std::make_shared<XZ_Rect>(-50.0f, 50.0f, -50.0f, 50.0f, 200.0f, light));
	world.Add(std::make_shared<XZ_Rect>(-500.0f, 500.0f, -500.0f, 500.0f, 0.0f, lambertian));
	world.Add(std::make_shared<Sphere>(Position3(0.0f, 50.0f, 0.0f), 50.0f, dieletrics));
	world.Add(std::make_shared<Sphere>(Position3(0.0f, 50.0f, 100.0f), 50.0f, metal));
	world.Add(std::make_shared<Sphere>(Position3(0.0f, 50.0f, -100.0f), 50.0f, ground));

	return world;
}

HitableList FinalScene()
{
	HitableList world;

	HitableList boxes;
	auto ground = std::make_shared<Lambertian>(Color3(1.0f, 1.0f, 1.0f));
	auto green = std::make_shared<Lambertian>(Color3(0.48f, 0.83f, 0.53f));

	const int boxes_per_side = 20;
	const float box_width = 100.0f;
	for(int x = 0; x < boxes_per_side; ++x)
		for (int z = 0; z < boxes_per_side; ++z)
		{
			Position3 Min(-1000.0f + x * box_width, 0.0f, -1000.0f + z * box_width);
			Position3 Max(Min.X + box_width, MathHelper::Random<float>(0.0f, 100.0f), Min.Z + box_width);
			auto checker = x % 2 == (z % 2 == 0 ? 0 : 1);
			auto color = ground;
			boxes.Add(std::make_shared<Box>(Min, Max, color));
		}
	world.Add(std::make_shared<BVHNode>(boxes));

	HitableList planes;
	auto white = std::make_shared<Lambertian>(Color3(0.73f, 0.73f, 0.73f));

	const int planes_per_side = 7;
	const float plane_width = 500.0f / planes_per_side;

	for (int x = 0; x < planes_per_side; ++x)
		for (int z = 0; z < planes_per_side; ++z)
		{
			float x0 = plane_width * x;
			float x1 = x0 + plane_width;
			float z0 = plane_width * z;
			float z1 = z0 + plane_width;

			if (x == 0 || x == planes_per_side - 1 || z == 0 || z == planes_per_side - 1)
			{
				planes.Add(std::make_shared<XZ_Rect>(x0, x1, z0, z1, 799.0f, white));
			}
			else
			{
				int checker = z % 2 == 0 ? 1 : 0;
				if (x % 2 == checker)
					planes.Add(std::make_shared<XZ_Rect>(x0, x1, z0, z1, 799.0f, white));
			}
		}
	world.Add(std::make_shared<BVHNode>(planes));

	
	auto top_light = std::make_shared<DiffuseLight>(Color3(0.8f, 0.8f, 8.0f));
	world.Add(std::make_shared<XZ_Rect>(0.0f, 500.0f, 0.0f, 500.0f, 800.0f, top_light));

	auto left_light = std::make_shared<DiffuseLight>(Color3(5.0f, 5.0f, 0.5f));
	world.Add(std::make_shared<Sphere>(Position3(800.0f, 200.0f, 200.0f), 100.0f, left_light));

	HitableList balls;
	auto dielectric = std::make_shared<Dielectrics>(1.5f);
	balls.Add(std::make_shared<Sphere>(Position3(170.0f, 450.0f, 200.0f), 100.0f, dielectric));

	auto metal = std::make_shared<Metal>(Color3(0.73f, 0.73f, 0.73f));
	balls.Add(std::make_shared<Sphere>(Position3(170.0f, 200.0f, 200.0f), 100.0f, metal));
	
	auto earth = std::make_shared<Lambertian>(std::make_shared<ImageTexture>(L"Resource/Texture/earth.png"));
	balls.Add(std::make_shared<Sphere>(Position3(320.0f, 250.0f, 450.0f), 150.0f, earth));

	auto movingball = std::make_shared<Lambertian>(Color3(0.73f, 0.3f, 0.73f));
	balls.Add(std::make_shared<MovingSphere>(Position3(-180.0f, 250.0f, 450.0f), Position3(-180.0f, 350.0f, 450.0f), 120.0f, movingball));

	auto r_light = std::make_shared<DiffuseLight>(Color3(3.0f, 0.5f, 0.5f));
	auto g_light = std::make_shared<DiffuseLight>(Color3(0.5f, 3.0f, 0.5f));
	auto b_light = std::make_shared<DiffuseLight>(Color3(0.5f, 0.5f, 3.0f));
	auto c_light = std::make_shared<DiffuseLight>(Color3(0.0f, 3.0f, 3.0f));
	auto m_light = std::make_shared<DiffuseLight>(Color3(3.0f, 0.0f, 3.0f));
	auto y_light = std::make_shared<DiffuseLight>(Color3(3.0f, 3.0f, 0.0f));

	balls.Add(std::make_shared<Sphere>(Position3(300.0f, 470.0f, 350.0f), 25.0f, r_light));
	balls.Add(std::make_shared<Sphere>(Position3(330.0f, 150.0f, 80.0f), 25.0f, y_light));
	balls.Add(std::make_shared<Sphere>(Position3(100.0f, 200.0f, 90.0f), 25.0f, b_light));
	balls.Add(std::make_shared<Sphere>(Position3(290.0f, 320.0f, 85.0f), 25.0f, m_light));
	balls.Add(std::make_shared<Sphere>(Position3(90.0f, 350.0f, 250.0f), 25.0f, c_light));
	balls.Add(std::make_shared<Sphere>(Position3(90.0f, 370.0f, -50.0f), 25.0f, g_light));
	

	world.Add(std::make_shared<BVHNode>(balls));

	auto boundary = std::make_shared<Sphere>(Position3(0.0f, 0.0f, 0.0f), 5000.0f, std::make_shared<Dielectrics>(1.5f));
	world.Add(std::make_shared<ConstantMedium>(boundary, Color3(1.0f, 1.0f, 1.0f), 0.0001f));

	return world;
}

void TitleScene(void);
void InitScene(void);
void WaitScene(void);
void RenderScene(void);

void TitleScene()
{
	static constexpr float offset_y = 50.0f;

	static char keystates[256] = {};
	static char oldstates[256] = {};
	static unsigned int frame = 0;
	

	for (int i = 0; i < 256; ++i)
	{
		oldstates[i] = keystates[i];
	}
	DxLib::GetHitKeyStateAll(keystates);

	if (keystates[KEY_INPUT_UP] && !oldstates[KEY_INPUT_UP])
		select_index = (num_scene + select_index - 1) % num_scene;
	if (keystates[KEY_INPUT_DOWN] && !oldstates[KEY_INPUT_DOWN])
		select_index = (select_index + 1) % num_scene;
	if (keystates[KEY_INPUT_RETURN] && !oldstates[KEY_INPUT_RETURN])
		scene = &WaitScene;

	DxLib::ClearDrawScreen();
	float offsetY = 100.0f;
	DxLib::DrawStringF(screen_width / 2.0f - 50.0f, offsetY, L"Select Scene", DxLib::GetColor(200, 200, 255));
	offsetY += 100.0f;
	float offsetX = 100.0f;
	int scene_index = 0;
	auto color = scene_index == select_index ? DxLib::GetColor(255, 255, 255) : DxLib::GetColor(150, 150, 150);
	DxLib::DrawStringF(offsetX, offsetY, L"Random Scene : 36 seconds", color);
	offsetY += offset_y;
	++scene_index;
	color = scene_index == select_index ? DxLib::GetColor(255, 255, 255) : DxLib::GetColor(150, 150, 150);
	DxLib::DrawStringF(offsetX, offsetY, L"Light Scene : 100 seconds", color);
	offsetY += offset_y;
	++scene_index;
	color = scene_index == select_index ? DxLib::GetColor(255, 255, 255) : DxLib::GetColor(150, 150, 150);
	DxLib::DrawStringF(offsetX, offsetY, L"Cornell Box Scene : 220 seconds", color);
	offsetY += offset_y;
	++scene_index;
	color = scene_index == select_index ? DxLib::GetColor(255, 255, 255) : DxLib::GetColor(150, 150, 150);
	DxLib::DrawStringF(offsetX, offsetY, L"Test Materials Scene : 50 seconds", color);
	offsetY += offset_y;
	++scene_index;
	color = scene_index == select_index ? DxLib::GetColor(255, 255, 255) : DxLib::GetColor(150, 150, 150);
	DxLib::DrawStringF(offsetX, offsetY, L"Cornell Box Volumetric Scene : 220 seconds", color);
	offsetY += offset_y;
	++scene_index;
	color = scene_index == select_index ? DxLib::GetColor(255, 255, 255) : DxLib::GetColor(150, 150, 150);
	DxLib::DrawStringF(offsetX, offsetY, L"Final Scene : 220 seconds", color);

	offsetY += 100.0f;
	if((++frame/3000) % 2 == 0)
		DxLib::DrawStringF(screen_width / 2.0f - 100.0f, offsetY, L"Press enter to select", DxLib::GetColor(200, 200, 255));
}

void WaitScene()
{
	DxLib::ClearDrawScreen();
	DxLib::DrawStringF(screen_width / 2.0f - 120.0f, screen_height / 2.0f, L"One thousand years later!", GetColor(255, 255, 255));

	static int frame = 0;

	if (++frame > 2000)
		scene = &InitScene;
}

void InitScene()
{
	switch (select_index)
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
		samples_per_pixel = 400;
		fov = 40.0f;
		text = { 1.0f,1.0f,1.0f };
		World = TestMaterialsScene();
		break;
	case 4:
		background = { 0.0f,0.0f,0.0f };
		look_from = { 278.0f, 278.0f, -800.0f };
		look_at = { 278.0f, 278.0f, 0.0f };
		max_bounce = 50;
		samples_per_pixel = 200;
		fov = 40.0f;
		text = { 1.0f,1.0f,1.0f };
		World = CornellBoxVolumetricScene();
		break;
	case 5:
		background = { 0.0f,0.0f,0.0f };
		look_from = { 478.0f, 278.0f, -600.0f };
		look_at = { 278.0f, 278.0f, 0.0f };
		max_bounce = 50;
		samples_per_pixel = 50;
		fov = 40.0f;
		text = { 1.0f,1.0f,1.0f };
		World = FinalScene();
		break;
	}

	camera = Camera(
		look_from,
		look_at,
		up,
		fov,
		aspect_ratio,
		aperture,
		focus_distance);

	scene = &RenderScene;
}

void RenderScene()
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
				float lengthU = static_cast<float>(x + MathHelper::Random<float>(-0.5f, 0.5f)) / (screen_width - 1);
				float lengthV = static_cast<float>(y + MathHelper::Random<float>(-0.5f, 0.5f)) / (screen_height - 1);

				color += RayColor(camera.GetRayAtScreenUV(lengthU, lengthV), background, World, max_bounce);
			}

			DrawPixel(x, y, GetColor(color, samples_per_pixel));
		}
	}
	float deltaTime = (GetNowCount() - time) / 1000.0f;
	DxLib::DrawFormatString(10, 10, GetColor(text), L"Elapsed time : %f seconds", deltaTime);
}

int main()
{
	ChangeWindowMode(true);
	SetGraphMode(screen_width, screen_height, color_bits);
	SetMainWindowText(_T("A FEW moments later"));
	DxLib_Init();

	scene = &TitleScene;

	while (!ProcessMessage())
	{
		(*scene)();
	}

	DxLib_End();
	return 0;
}
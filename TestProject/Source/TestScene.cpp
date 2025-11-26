#include "TestScene.h"
#include "TestComponent.h"
#include "CameraController.h"
#include "TransformGUI.h"

void TestScene::Init()
{
	{
		auto obj = CreateGameObject();
		//obj->GetTransform()->SetPosition(0.0f, 8.0f, -30.0f);
		obj->GetTransform()->SetPosition(0.0f, 0.0f, -10.0f);
		//obj->GetTransform()->SetEulerAngle(20.0f, 0.0f, 0.0f);
		obj->AddComponent<Camera>();
		obj->AddComponent<CameraController>();
		obj->AddComponent<TransformGUI>()->SetName("Camera");
	}

	{
		auto obj = CreateGameObject();
		obj->GetTransform()->SetEulerAngle(50.0f, -30.0f, 0.0f);
		auto light = obj->AddComponent<DirectionalLight>();
	}

	{
		auto obj = CreateGameObject();
		obj->GetTransform()->SetPosition(5.0f, -2.0f, 0.0f);
		auto renderer = obj->AddComponent<SpriteRenderer>();
		renderer->SetUI(true);
		renderer->LoadTexture("Assets/Textures/UI.png");
		renderer->SetSize(100.0f,200.0f);
	}

	{
		auto obj = CreateGameObject();
		obj->GetTransform()->SetPosition(0.0f, 1.0f, 0.0f);
		auto renderer = obj->AddComponent<SpriteRenderer>();
		renderer->SetUI(false);
		renderer->LoadTexture("Assets/Textures/Icon_Map.png");
		renderer->SetSize(500.0f);
		renderer->SetAntiAliasing(false);
	}

	for (int z = -10; z <= 10; ++z)
	{
		for (int x = -5; x <= 5; ++x)
		{
			auto obj = CreateGameObject();
			auto transform = obj->GetTransform();
			transform->SetPosition(x * 2.0f, -1.0f, z * 2.0f);
			if (std::fabsf(x) + std::fabsf(z * 0.5f) > 2)
			{
				int randNum = rand() % 20;

				if (randNum < 1)
				{
					transform->SetScale(1.0f, 1.5f, 1.0f);
					transform->Translate(0.0f, 0.5f, 0.0f);
				}
				else if (randNum < 3)
				{
					transform->SetScale(1.0f, 1.25f, 1.0f);
					transform->Translate(0.0f, 0.25f, 0.0f);
				}
			}
			auto renderer = obj->AddComponent<MeshRenderer>();
			renderer->LoadModel("Assets/Model/dirt_with_grass.fbx");
		}
	}

	{
		Vector3 treePos[12] =
		{
			{10.0f, 0.0f, 18.0f},
			{10.0f, 0.0f, -10.0f},
			{9.0f, 0.0f, 6.0f},
			{9.0f, 0.0f, -6.0f},
			{8.0f, 0.0f, 12.0f},
			{8.0f, 0.0f, -12.0f},
			{-10.0f, 0.0f, -18.0f},
			{-10.0f, 0.0f, 10.0f},
			{-9.0f, 0.0f, -6.0f},
			{-9.0f, 0.0f, 6.0f},
			{-8.0f, 0.0f, -12.0f},
			{-8.0f, 0.0f, 12.0f}
		};

		for (int i = 0; i < 12; ++i)
		{
			auto obj = CreateGameObject();
			auto transform = obj->GetTransform();
			transform->SetPosition(treePos[i]);
			auto renderer = obj->AddComponent<MeshRenderer>();
			renderer->LoadModel("Assets/Model/tree.fbx");
		}
	}
}

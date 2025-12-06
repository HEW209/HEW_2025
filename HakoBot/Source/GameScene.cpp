#include "GameScene.h"
#include "../test.h"
#include <GameFrame/RenderSystem.h>
#include "SampleCameraController.h"

void GameScene::Init()
{
	RenderSystem::Instance().SetClearColor(Color(0.0f, 0.0f, 0.0f, 1.0f));

	{
		auto obj = CreateGameObject();
		obj->GetTransform()->SetPosition(0.0f, 2.0f, -10.0f);
		obj->AddComponent<Camera>();

		auto cameraRoot = CreateGameObject();
		cameraRoot->AddComponent<SampleCameraController>();
		obj->GetTransform()->SetParent(cameraRoot);
	}

	{
		auto obj = CreateGameObject();
		obj->GetTransform()->SetPosition(0.0f, 3.0f, -4.0f);
		obj->AddComponent<MeshRenderer>();
	}

	{
		auto obj = CreateGameObject();
		obj->GetTransform()->SetEulerAngle(50.0f, -30.0f, 0.0f);
		auto light = obj->AddComponent<DirectionalLight>();
	}

	{
		auto obj = CreateGameObject();
		obj->GetTransform()->SetScale(3.0f, 3.0f, 3.0f);

		const char* fileName[] =
		{
			"Assets/Model/Player/fbx/jyoubu.fbx",
			"Assets/Model/Player/fbx/kabu.fbx",
			"Assets/Model/Player/fbx/sityu.fbx",
			"Assets/Model/karubi/‚Ü‚¯‚ñ®ÔŒ©‚©‚é‚Ñ•Pˆß‘•_ƒ}ƒ“ƒg‚È‚µver1.0.pmx"
		};

		for (int i = 0; i < 3; ++i)
		{
			auto renderer = obj->AddComponent<MeshRenderer>();
			renderer->LoadModel(fileName[i]);
			auto materials = renderer->GetMaterials();
			for (auto& mat : *materials)
			{
				mat.SetPixelShader("Assets/Shader/Hologram_PS.cso");
				mat.SetBlendState(BlendState::ALPHA);
				mat.SetDepthStencilState(DepthStencilState::READ_ONLY);
				mat.SetRasterizerState(RasterizerState::NONE);
			}
		}
		obj->AddComponent<test>();
	}
}

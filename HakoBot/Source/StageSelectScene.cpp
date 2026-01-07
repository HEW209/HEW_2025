#include"StageSelectScene.h"
#include"StageNumber.h"
#include"StageSelectObject.h"
#include"StageSelectConveyors.h"
#include "BlockObject.h"

#include "LevelSerializer.h"

void StageSelectScene::Init()
{
	{
		auto obj = CreateGameObject();
		auto camera = obj->AddComponent<Camera>();
		obj->GetTransform()->SetPosition(0.0f, 0.0f, -10.0f);
	}
	{
		auto lightObj = CreateGameObject();
		auto light = lightObj->AddComponent<DirectionalLight>();
		lightObj->GetTransform()->SetEulerAngle(50.0f, -30.0f, 0.0f);
	}

	auto numberObj = CreateGameObject();
	auto stageNumber = numberObj->AddComponent<StageNumber>();
	{
		auto obj = CreateGameObject();
		auto selectObject = obj->AddComponent<StageSelectObject>();
		selectObject->SetStageNumber(stageNumber);
	}
	{
		auto obj = CreateGameObject();

		auto mesh = obj->AddComponent<MeshRenderer>();
		mesh->LoadModel("Assets/Model/Stage/FBX/Conveyor_frame.fbx");
		obj->GetTransform()->SetPosition(0.0f, -6.3f, 0.0f);
		obj->GetTransform()->SetScale(1.5f, 1.0f, 2.1f);

	}

	{
		auto obj = CreateGameObject();
		auto Textures = obj->AddComponent<SpriteRenderer>();
		Textures->LoadTexture("Assets/Textures/result!.png");
		obj->GetTransform()->SetScale(0.25f, 0.25f, 0.25f);
		obj->GetTransform()->SetPosition(3.0f, 1.5f, 0.0f);
	}
	{
		auto obj = CreateGameObject();
		auto Textures = obj->AddComponent<SpriteRenderer>();
		Textures->LoadTexture("Assets/Textures/result!.png");
		obj->GetTransform()->SetScale(0.2f, 0.1f, 0.2f);
		obj->GetTransform()->SetPosition(-4.0f, 3.0f, 0.0f);
	}
}




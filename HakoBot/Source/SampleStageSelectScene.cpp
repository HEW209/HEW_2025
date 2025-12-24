#include"SampleStageSelectScene.h"
#include"StageNumber.h"
#include"StageSelectObject.h"
#include "BlockObject.h"

#include "LevelSerializer.h"

void StageSelectScene::Init()
{
	{
		auto obj = CreateGameObject();
		auto camera = obj->AddComponent<Camera>();
		obj->GetTransform()->SetPosition(0.0f, 0.0f, -10.0f);
	}	
	for (int i = 0; i < 30; i++)
	{


		auto obj = CreateGameObject();
		auto SelectObject = obj->AddComponent<StageSelectObject>();
		SelectObject->SetStageID(i);

		auto mesh = obj->AddComponent<MeshRenderer>();
		mesh->LoadModel(SelectObject->SetModelID());

		obj->GetTransform()->SetPosition(20.0f, -2.0f, 0.0f);
	}
	{
		auto lightObj = CreateGameObject();
		auto light = lightObj->AddComponent<DirectionalLight>();
		lightObj->GetTransform()->SetEulerAngle(50.0f, -30.0f, 0.0f);
	}
    
	{
		auto obj = CreateGameObject();
		auto SelectNumber = obj->AddComponent<StageNumber>();
		
	}
	/*
	{
		auto obj = CreateGameObject();
		auto mesh = obj->AddComponent<MeshRenderer>();
		auto a = obj->AddComponent<SpriteRenderer>();
		mesh->LoadModel("Assets/Model/Stage/FBX/Conveyors.fbx");
		
		a->LoadTexture("Assets/Model/Stage/texture/Conveyors.png");
		obj->GetTransform()->SetPosition(0.0f, -4.5f, 0.0f);

	}
	*/
}




#include "GameScene.h"
#include "Easing.h"
#include "GameFrame/Component.h"
#include "GuideUIController.h"
#include "GuideUIController2.h"
#include "GuideUITimeController.h"
#include "GuideUITimeController2.h"
#include "GuideUIResultController.h"




void GameScene::Init()
{

	//カメラ
	{
		auto obj = CreateGameObject();
		auto camera = obj->AddComponent<Camera>();
	}

	//UIオブジェクトを作成
	CreateUIObject();
}




void GameScene::CreateUIObject()
{
	//メニュー
	{
		auto obj = CreateGameObject();
		auto renderer = obj->AddComponent<SpriteRenderer>();
		renderer->SetUI(true);
		renderer->LoadTexture("Assets/Textures/menuu.png");
		renderer->GetTransform()->SetPosition(-5.6f, 3.1f, 0.0f);
		renderer->SetSize(110.0f, 110.0f);

	}

	//おく
	{
		auto obj = CreateGameObject();
		auto renderer = obj->AddComponent<SpriteRenderer>();
		renderer->SetUI(true);
		renderer->LoadTexture("Assets/Textures/okuB.png");
		renderer->GetTransform()->SetPosition(5.6f, -1.6f, 0.0f);
		renderer->SetSize(MOZI_SIZE + 70.0f, MOZI_SIZE + 20.0f);
		obj->AddComponent<GuideUIController>();
	}

	//完成 
	{
		auto obj = CreateGameObject();
		auto renderer = obj->AddComponent<SpriteRenderer>();
		renderer->SetUI(true);
		renderer->LoadTexture("Assets/Textures/kanbansei.png");
		renderer->GetTransform()->SetPosition(4.3f, -4.6f, 0.0f);
		renderer->SetSize(MOZI_SIZE + 150.0f, MOZI_SIZE + 100.0f);
		obj->AddComponent<GuideUIController2>();
	}

	//メニュー開いたやつ
	{
		auto obj = CreateGameObject();
		obj->AddComponent<GuideUIResultController>();
	}

	//タイマー1
	{
		auto obj = CreateGameObject();

		obj->GetTransform()->SetPosition(4.1f, 3.2f, 0.0f);

		obj->AddComponent<GuideUITimeController>();
	}

	//リザルト
	{
		auto obj = CreateGameObject();
		auto renderer = obj->AddComponent<SpriteRenderer>();
		renderer->SetUI(true);
		renderer->LoadTexture("Assets/Textures/result3.png");
		renderer->GetTransform()->SetPosition(0.0f, 0.0f, 0.0f);
		obj->GetTransform()->SetScale(0.0f, 0.0f, 0.0f);
		obj->AddComponent<GuideUITimeController2>();
	}
}
	


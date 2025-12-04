#include "GameScene.h"
#include "Easing.h"
#include "GameFrame/Component.h"
#include "GuideUIController.h"




void GameScene::Init()
{
	

	//カメラ
	{
		auto obj = CreateGameObject();
		auto camera = obj->AddComponent<Camera>();
	}

	//メニューの十字
	{
		auto obj = CreateGameObject();
		auto renderer = obj->AddComponent<SpriteRenderer>();
		renderer->SetUI(true);
		renderer->LoadTexture("Assets/Textures/menuplus.png");
		renderer->GetTransform()->SetPosition(-4.2f, 1.7f, 0.0f);
		renderer->SetSize(400.0f, 400.0f);
	
	}

	//メニュー文字
	{
		auto obj = CreateGameObject();
		auto renderer = obj->AddComponent<SpriteRenderer>();
		renderer->SetUI(true);
		renderer->LoadTexture("Assets/Textures/menu.png");
		renderer->GetTransform()->SetPosition(-3.9f, 1.5f, 0.0f);
		renderer->SetSize(450.0f, 450.0f);
	
	}
	{
		auto obj = CreateGameObject();
		auto renderer = obj->AddComponent<SpriteRenderer>();
		renderer->SetUI(true);
		renderer->LoadTexture("Assets/Textures/oku.png");
		renderer->GetTransform()->SetPosition(5.6f, -1.9f, 0.0f);
		renderer->SetSize(MOZI_SIZE, MOZI_SIZE);
		obj->AddComponent<GuideUIController>();
	}

	{
		auto obj = CreateGameObject();
		auto renderer = obj->AddComponent<SpriteRenderer>();
		renderer->SetUI(true);
		renderer->LoadTexture("Assets/Textures/oku.png");
		renderer->GetTransform()->SetPosition(5.6f, -1.9f, 0.0f);
		renderer->SetSize(MOZI_SIZE, MOZI_SIZE);
		obj->AddComponent<GuideUIController>();
	}
}


	
	


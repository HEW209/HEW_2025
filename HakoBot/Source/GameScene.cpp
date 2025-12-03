#include "GameScene.h"
#include "Easing.h"
#include "GameFrame/Component.h"

SpriteRenderer* Renderer1 = nullptr;
Component* compornent;

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
		renderer->LoadTexture("Assets/Textures/sprite09.png");
		renderer->GetTransform()->SetPosition(0.0f, 0.0f, 0.0f);
		renderer->SetSize(450.0f, 450.0f);
		Renderer1 = renderer;
	}
	
}

void GameScene::Update()
{
	
	Renderer1->GetTransform()->SetPosition(Easing::InSine(0.0f, 100.0f, 1.0f, 2.0f), 1.5f, 0.0f);
	

	Renderer1->SetSize(450.0f, 450.0f);
}

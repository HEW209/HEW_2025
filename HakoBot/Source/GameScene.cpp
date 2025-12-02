#include "GameScene.h"


void GameScene::Init()
{
	{
		auto obj = CreateGameObject();
		auto camera = obj->AddComponent<Camera>();
	}
	{
		auto obj = CreateGameObject();
		auto renderer = obj->AddComponent<SpriteRenderer>();
		renderer->SetUI(true);
		obj->GetTransform()->SetPosition(0.0f, 0.0f, 0.0f);
		renderer->LoadTexture("Assets/Textures/menuplus.png");
		renderer->SetSize(500.0f, 500.0f);
	}

	
}

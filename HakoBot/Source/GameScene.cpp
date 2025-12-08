#include "GameScene.h"
#include "Easing.h"
#include "GameFrame/Component.h"
#include "GuideUIController.h"
#include "GuideUIController2.h"
#include "GuideUITimeController.h"




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
		K = true;
		auto obj = CreateGameObject();
		auto renderer = obj->AddComponent<SpriteRenderer>();
		renderer->SetUI(true);
		if (!K)
		{
			renderer->LoadTexture("Assets/Textures/kansei!.png");
			renderer->GetTransform()->SetPosition(7.6f, -2.9f, 0.0f);
			renderer->SetSize(MOZI_SIZE + 150.0f, MOZI_SIZE + 20.0f);
			obj->AddComponent<GuideUIController2>();
		}
		else
		{
			renderer->LoadTexture("Assets/Textures/kanbansei.png");
			renderer->GetTransform()->SetPosition(4.3f, -4.5f, 0.0f);
			renderer->SetSize(MOZI_SIZE + 150.0f, MOZI_SIZE + 100.0f);
			obj->AddComponent<GuideUIController2>();
		}
		
	}

	{
		auto obj = CreateGameObject();
		//auto renderer = obj->AddComponent<SpriteRenderer>();
		obj->AddComponent<GuideUITimeController>();
	}

}


bool GameScene::GetK()
{
	return K;
}
	


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

	//メニューの十字
	{
		auto obj = CreateGameObject();
		auto renderer = obj->AddComponent<SpriteRenderer>();
		renderer->SetUI(true);
		renderer->LoadTexture("Assets/Textures/menu!.png");
		renderer->GetTransform()->SetPosition(-5.6f, 3.1f, 0.0f);
		renderer->SetSize(110.0f, 110.0f);
	
	}

	//メニュー文字
	/*{
		auto obj = CreateGameObject();
		auto renderer = obj->AddComponent<SpriteRenderer>();
		renderer->SetUI(true);
		renderer->LoadTexture("Assets/Textures/menu.png");
		renderer->GetTransform()->SetPosition(-3.9f, 1.5f, 0.0f);
		renderer->SetSize(450.0f, 450.0f);
	
	}*/

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

	//完成 Kがtrueなら看板、falseなら初期案 falseうごかなくした
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
			renderer->GetTransform()->SetPosition(4.3f, -4.6f, 0.0f);
			renderer->SetSize(MOZI_SIZE + 150.0f, MOZI_SIZE + 100.0f);
			obj->AddComponent<GuideUIController2>();
		}
		
	}

	//リザルト
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

	////タイマー2
	//{
	//	auto obj = CreateGameObject();
	//	auto renderer = obj->AddComponent<SpriteRenderer>();
	//	renderer->SetUI(true);
	//	renderer->LoadTexture("Assets/Textures/sprite.png");
	//	renderer->GetTransform()->SetPosition(4.0f, 3.2f, 0.0f);
	//	renderer->SetUVScale(1.0f / 6.0f, 1.0f / 2.0f);
	//	renderer->SetSize(MOZI_SIZE + 50.0f, MOZI_SIZE + 50.0f);
	//	obj->AddComponent<GuideUITimeController2>();
	//}

}


bool GameScene::GetK()
{
	return K;
}
	


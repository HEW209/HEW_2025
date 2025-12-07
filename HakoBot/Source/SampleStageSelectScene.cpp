#include"SampleStageSelectScene.h"

void StageSelectScene::Init()
{
	{
		auto obj = CreateGameObject();
		auto camera = obj->AddComponent<Camera>();
	}
	{
		auto obj = CreateGameObject();
		auto rendere = obj->AddComponent<SpriteRenderer>();
		rendere->SetUI(true);
		rendere->SetOffsetPos(0, 0);
		auto key = obj->AddComponent<StegeSelectKeyEnter>();
		
	}
}


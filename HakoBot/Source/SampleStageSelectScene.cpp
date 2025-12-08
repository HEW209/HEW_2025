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
		rendere->SetOffsetPos(-5.8f,3.0f);
	
		
	}
}


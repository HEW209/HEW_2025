#include "DebugEffectScene.h"

DebugEffectScene::DebugEffectScene()
{

}

void DebugEffectScene::Init()
{
	{
		
		auto obj = CreateGameObject();
		auto camera = obj->AddComponent<Camera>();
		obj->GetTransform()->SetPosition(2.0f, 4.0f, -10.0f);
		
	}
	{
		auto obj = CreateGameObject();
		auto effect = obj->AddComponent<EffectRenderer>();
		effect->Load("Assets/Effect/mizusibuki.efkefc");
		effect->Play();
	}

}

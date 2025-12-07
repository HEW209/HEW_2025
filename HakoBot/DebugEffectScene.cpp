#include "DebugEffectScene.h"

DebugEffectScene::DebugEffectScene()
{

}

void DebugEffectScene::Init()
{
	{
		auto obj = CreateGameObject();
		obj->AddComponent<Camera>();
	}

	{
		auto obj = CreateGameObject();
		auto effeksser = obj->AddComponent<EffeksserDebug>();
		auto manager = effeksser->GetManager();
		/*Effekseer::Effect::Create(manager, L"assets/Laser01.efkefc")
		 m_loadedEffect = Effekseer::Effect::Create(manager,L"assets/Laser01.efkefc");*/
	}

}

#include "DebugEffectScene.h"

DebugEffectScene::DebugEffectScene()
{

}

void DebugEffectScene::Init()
{

	{
		auto obj = CreateGameObject();
		auto camera = obj->AddComponent<Camera>();
		auto transform = camera->GetTransform();
		transform->SetPosition(Vector3(5.0f, 4.0f, 0.0f));


	}

	{

		auto obj = CreateGameObject();

	}

}

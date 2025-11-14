#include "GridDebugScene.h"

#include "GridDebugSceneManager.h"
#include "SampleCamera.h"

void GridDebugScene::Init()
{
	//カメラ
	{
		auto obj = CreateGameObject();
		auto camera = obj->AddComponent<Camera>();
		auto transform = obj->GetTransform();
		transform->SetEulerAngle(20.0f, 0.0f, 0.0f);
		obj->AddComponent<SampleCamera>();
	}

	// シーンマネージャー
	{
		auto obj = CreateGameObject();
		obj->AddComponent<GridDebugSceneManager>();
	}
}
#include "GridDebugScene.h"

#include "BlockObject.h"
#include "SampleCamera.h"

void GridDebugScene::Init()
{
	//カメラ
	{
		auto obj = CreateGameObject();
		auto camera = obj->AddComponent<Camera>();
		auto transform = obj->GetTransform();
		transform->SetEulerAngle(20.0f, 45.0f, 0.0f);
		obj->AddComponent<SampleCamera>();
	}

	// ブロック
	{
		auto obj = CreateGameObject();
		obj->AddComponent<BlockObject>();
	}
}
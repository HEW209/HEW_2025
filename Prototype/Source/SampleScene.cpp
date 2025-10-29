#include "SampleScene.h"
#include "SamplePlayer.h"
#include "SampleCamera.h"

void SampleScene::Init()
{
	//カメラ
	{
		auto obj = CreateGameObject();
		auto camera = obj->AddComponent<Camera>();
		auto transform = obj->GetTransform();
		transform->SetEulerAngle(30.0f, 45.0f, 0.0f);
		obj->AddComponent<SampleCamera>();
	}

	//プレイヤー
	{
		auto obj = CreateGameObject();
		auto renderer = obj->AddComponent<MeshRenderer>();
		obj->AddComponent<SamplePlayer>();
	}

	//床
	{
		auto obj = CreateGameObject();
		auto renderer = obj->AddComponent<MeshRenderer>();
		auto transform = obj->GetTransform();
		transform->m_position.y = -1.0f;
		transform->m_scale = { 10.0f, 1.0f, 10.0f };
	}
}
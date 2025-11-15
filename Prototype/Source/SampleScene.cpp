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
		transform->SetEulerAngle(20.0f, 45.0f, 0.0f);
		obj->AddComponent<SampleCamera>();
	}

	//プレイヤー
	{
		auto obj = CreateGameObject();
		auto renderer = obj->AddComponent<MeshRenderer>();
		//renderer->LoadModel("Assets/Model/karubi/まけん式赤見かるび姫衣装ver1.0.pmx");
		obj->AddComponent<Collider>();
		obj->AddComponent<SamplePlayer>();
		auto transform = obj->GetTransform();
		transform->m_scale = { 1.0f, 1.0f, 1.0f };
		transform->m_position = { 0.0f, 2.0f, 0.0f };
		//transform->m_scale = { 0.1f, 0.1f, 0.1f };
	}

	//床
	{
		auto obj = CreateGameObject();
		auto renderer = obj->AddComponent<MeshRenderer>();
		auto transform = obj->GetTransform();
		transform->m_position.y = -0.5f;
		transform->m_scale = { 1.0f, 1.0f, 1.0f };
		obj->AddComponent<Collider>();
	}

}
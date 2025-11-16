#include "SampleScene.h"
#include "SamplePlayer.h"
#include "SampleCamera.h"

void SampleScene::Init()
{
	// ライト
	{
		auto obj = CreateGameObject();
		auto light = obj->AddComponent<DirectionalLight>();
		light->SetLightColor({ 0.4f, 0.7f, 1.0f, 1.0f });
		//light->SetLightColor({ 1.0f, 0.7f, 0.4f, 1.0f });
		light->SetLightColor({ 0.4f, 1.0f, 0.4f, 1.0f });
		light->SetAmbientColor({ 0.6f, 0.6f, 0.6f, 1.0f });
		auto transform = obj->GetTransform();
		transform->SetEulerAngle(10.0f, -30.0f, 0.0f);
	}

	// カメラ
	{
		auto obj = CreateGameObject();
		auto camera = obj->AddComponent<Camera>();
		auto transform = obj->GetTransform();
		transform->SetEulerAngle(20.0f, 45.0f, 0.0f);
		obj->AddComponent<SampleCamera>();
	}

	// プレイヤー
	{
		auto parent = CreateGameObject();
		auto obj = CreateGameObject();
		auto renderer = obj->AddComponent<MeshRenderer>();
		renderer->LoadModel("Assets/Model/karubi/まけん式赤見かるび姫衣装ver1.0.pmx");
		obj->AddComponent<SamplePlayer>();
		auto transform = obj->GetTransform();
		transform->m_scale = { 0.3f, 0.3f, 0.3f };
		transform->m_position = { 0.0f, 5.0f, 0.0f };
		transform->SetParent(parent->GetTransform());
		parent->GetTransform()->SetEulerAngle(0.0f, 0.0f, 0.0f);
		parent->GetTransform()->m_scale = { 1.0f,1.0f,1.0f };
		parent->AddComponent<MeshRenderer>();

		auto child = CreateGameObject();
		child->GetTransform()->SetParent(obj->GetTransform());
		child->AddComponent<MeshRenderer>();
	}

	// 床
	{
		auto obj = CreateGameObject();
		auto renderer = obj->AddComponent<MeshRenderer>();
		auto transform = obj->GetTransform();
		transform->m_position.y = -0.5f;
		transform->m_scale = { 10.0f, 1.0f, 10.0f };
	}

}
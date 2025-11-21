#include "CollisionDebugScene.h"
#include "SampleCamera.h"
#include "PlayerMove.h"

void CollisionDebugScene::Init()
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
	// 床
	{
		auto obj = CreateGameObject();
		auto renderer = obj->AddComponent<MeshRenderer>();
		auto transform = obj->GetTransform();
		transform->m_position.y = 0.5f;
		transform->m_scale = { 10.0f, 1.0f, 10.0f };
		transform->SetEulerAngle(0.0f, 0.0f, 0.0f);
		auto collider = obj->AddComponent<Collider>();
		collider->m_scale = { 10.0f, 1.0f, 10.0f };
	}
	// プレイヤー
	{
		auto obj = CreateGameObject();
		auto collider = obj->AddComponent<Collider>();
		collider->IsStatic = false;
		obj->AddComponent<PlayerMove>();
		auto transform = obj->GetTransform();
		transform->m_scale = { 1.0f, 1.0f, 1.0f };
		transform->m_position = { 2.0f, 10.0f, 0.0f };
		obj->AddComponent<MeshRenderer>();
	}

	
	// 壁
	{
		auto obj = CreateGameObject();
		auto renderer = obj->AddComponent<MeshRenderer>();
		auto transform = obj->GetTransform();
		transform->m_position.y = 1.5f;
		transform->SetEulerAngle(0.0f, 0.0f, 0.0f);
		auto collider = obj->AddComponent<Collider>();
	}

	// 壁
	{
		auto obj = CreateGameObject();
		auto renderer = obj->AddComponent<MeshRenderer>();
		auto transform = obj->GetTransform();
		transform->m_position = {1.0f,1.5f ,1.0f};
		transform->SetEulerAngle(0.0f, 0.0f, 0.0f);
		auto collider = obj->AddComponent<Collider>();
	}
	// 壁
	{
		auto obj = CreateGameObject();
		auto renderer = obj->AddComponent<MeshRenderer>();
		auto transform = obj->GetTransform();
		transform->m_position = { -3.0f,1.5f ,-3.0f };
		transform->SetEulerAngle(0.0f, 30.0f, 0.0f);
		auto collider = obj->AddComponent<Collider>();
	}
}
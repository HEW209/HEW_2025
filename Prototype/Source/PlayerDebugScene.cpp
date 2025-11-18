#include "PlayerDebugScene.h"
#include "PlayerMove.h"
#include "SampleCamera.h"
//グリッド
#include "GameState.h"
#include "Player.h"

#include "PlayerBlockHandler.h"

void PlayerDebugScene::Init()
{
	// ゲームステート
	{
		auto obj = CreateGameObject();
		obj->AddComponent<GameState>();
	}
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
		obj->AddComponent<Player>();
	}

	// 壁
	{
		auto obj = CreateGameObject();
		auto renderer = obj->AddComponent<MeshRenderer>();
		obj->AddComponent<Collider>();
		auto transform = obj->GetTransform();
		transform->m_position.y = 0.5f;
		transform->m_scale = { 1.0f, 1.0f, 1.0f };
	}

	{//グリッドフィールドの作成

		auto obj = CreateGameObject();
		auto component = obj->AddComponent<GridField>();
		GameState::GetInstance()->SetGridField(component);
		component->SetSize({ 4, 4, 4 });
		obj->GetTransform()->SetPosition(0.0f, 0.0f, 0.0f);

	}

	{
		auto obj = CreateGameObject();
		auto renderer = obj->AddComponent<MeshRenderer>();
		auto transform = obj->GetTransform();
		transform->SetScale(4.0f, 0.4f, 4.0f);
		//グリッドフィールドの下
		transform->SetPosition(0.0f, -0.2f, 0.0f);

	}

	{//ライト
		auto obj = CreateGameObject();
		obj->AddComponent<DirectionalLight>();
		obj->GetTransform()->SetEulerAngle(50.0f, -30.0f, 0.0f);
	}
}

























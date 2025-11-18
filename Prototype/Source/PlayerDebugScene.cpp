#include "PlayerDebugScene.h"
#include "PlayerMove.h"
#include "SampleCamera.h"
//グリッド
#include "GameState.h"

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
		CreatePlayer();
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

void PlayerDebugScene::CreatePlayer()
{
	{
		//移動できるオブジェクト（プレイヤー）を作成
		//移動できるプレイヤーオブジェクトを作る
		auto playerRoot = CreateGameObject();

		//プレイヤールートにコンポーネントを追加
		auto renderer2 = playerRoot->AddComponent<MeshRenderer>();
		renderer2->LoadModel("Assets/Model/HEW/FBX/kabu.fbx");

		playerRoot->AddComponent<PlayerMove>();
		auto collider = playerRoot->AddComponent<Collider>();
		collider->IsStatic = false;

		// 頭作る	
		auto playerHead = CreateGameObject();

		//頭にコンポーネントを追加
		auto renderer1 = playerHead->AddComponent<MeshRenderer>();
		renderer1->LoadModel("Assets/Model/HEW/FBX/jyoubu.fbx");
		playerHead->GetTransform()->SetParent(playerRoot->GetTransform());

		//ブロック操作コンポーネントの追加
		auto blockHandler = playerHead->AddComponent<PlayerBlockHandler>();

		// 支柱作る
		auto playerPillar = CreateGameObject();

		// 支柱にコンポーネントつける
		auto renderer3 = playerPillar->AddComponent<MeshRenderer>();
		renderer3->LoadModel("Assets/Model/HEW/FBX/sityu.fbx");
		playerPillar->GetTransform()->SetParent(playerRoot->GetTransform());
		playerPillar->GetTransform()->SetScale(1.0f, 1.0f, 1.0f);

		//ブロックオブジェクトを生成
		auto blockObj = CreateGameObject();

		//オフセットで位置を変える
		blockObj->GetTransform()->SetPosition(0.0f, 2.0f, 0.0f);
		auto blockComponent = blockObj->AddComponent<BlockObject>();
		blockHandler->SetBlockObject(blockComponent);
	}
}

































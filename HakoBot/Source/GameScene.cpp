#include "GameScene.h"

#include  "InputSystem.h"
//カメラ用
#include "SampleCamera.h"
//グリッド
#include "GameState.h"
#include "Player.h"
#include "PlayerBlockHandler.h"
#include "PlayerCamera.h"

void GameScene::Init()
{
	KeyBind();

	//移動できるオブジェクト（プレイヤー）を作成	
	//移動できるプレイヤーオブジェクトを作る
	auto player = CreateGameObject();
	player->AddComponent<Player>();

	//カメラ
	{
		auto Obj = CreateGameObject();
		auto playerCamera = Obj->AddComponent<PlayerCamera>();
		playerCamera->SetPlayer(player->GetTransform());
		playerCamera->SetCameraDistance(20.0f);
	}

	// シーンマネージャー
	{
		auto obj = CreateGameObject();
		obj->AddComponent<GameState>();
	}

	CreateStageSet();
	CreateGridField();

	//ワールドに配置するブロック
	{
		BlockSetData blockSet;
		blockSet.blocks.resize(1);
		blockSet.blocks[0] = { 0, 0, 0 };

		auto obj = CreateGameObject();
		auto component = obj->AddComponent<BlockObject>();
		component->SetBlockSet(blockSet);
		auto transform = obj->GetTransform();
		transform->SetPosition(Vector3{ 8.0f, 0.0f, 5.0f } + component->GetGroundOffset());

		GameState::GetInstance()->AppendWorldBlock(component);
	}
	{
		BlockSetData blockSet;
		blockSet.blocks.resize(2);
		blockSet.blocks[0] = { 0, 0, 0 };
		blockSet.blocks[1] = { 1, 0, 0 };

		auto obj = CreateGameObject();
		auto component = obj->AddComponent<BlockObject>();
		component->SetBlockSet(blockSet);
		auto transform = obj->GetTransform();
		transform->SetPosition(Vector3{ 8.0f, 0.0f, 2.0f } + component->GetGroundOffset());

		GameState::GetInstance()->AppendWorldBlock(component);
	}
	{
		BlockSetData blockSet;
		blockSet.blocks.resize(3);
		blockSet.blocks[0] = { 0, 0, 0 };
		blockSet.blocks[1] = { 1, 0, 0 };
		blockSet.blocks[2] = { 0, 0, 1 };

		auto obj = CreateGameObject();
		auto component = obj->AddComponent<BlockObject>();
		component->SetBlockSet(blockSet);
		auto transform = obj->GetTransform();
		transform->SetPosition(Vector3{ 8.0f, 0.0f, -2.0f } + component->GetGroundOffset());

		GameState::GetInstance()->AppendWorldBlock(component);
	}
	{
		BlockSetData blockSet;
		blockSet.blocks.resize(4);
		blockSet.blocks[0] = { 0, 0, 0 };
		blockSet.blocks[1] = { 1, 0, 0 };
		blockSet.blocks[2] = { 0, 1, 0 };
		blockSet.blocks[3] = { 0, 0, -1 };

		auto obj = CreateGameObject();
		auto component = obj->AddComponent<BlockObject>();
		component->SetBlockSet(blockSet);
		auto transform = obj->GetTransform();
		transform->SetPosition(Vector3{ 8.0f, 0.0f, -5.0f } + component->GetGroundOffset());

		GameState::GetInstance()->AppendWorldBlock(component);
	}

	{//ライト
		auto obj = CreateGameObject();
		obj->AddComponent<DirectionalLight>();
		obj->GetTransform()->SetEulerAngle(50.0f, -30.0f, 0.0f);
	}

}

void GameScene::KeyBind()
{
	InputSystem::CreateButtonAction("RotateBlockUp"_hash);
	InputSystem::CreateButtonAction("RotateBlockDown"_hash);
	InputSystem::CreateButtonAction("RotateBlockLeft"_hash);
	InputSystem::CreateButtonAction("RotateBlockRight"_hash);
	InputSystem::CreateButtonAction("PlaceAndRemove"_hash);
	InputSystem::CreateButtonAction("Up"_hash);
	InputSystem::CreateButtonAction("Down"_hash);
	InputSystem::CreateButtonAction("CameraLeft"_hash);
	InputSystem::CreateButtonAction("CameraRight"_hash);
	InputSystem::CreateAxisAction("Move"_hash);
	InputSystem::CreateAxisAction("CameraMove"_hash);

	InputSystem::BindKey("RotateBlockUp"_hash, KeyCode::UP);
	InputSystem::BindKey("RotateBlockDown"_hash, KeyCode::DOWN);
	InputSystem::BindKey("RotateBlockLeft"_hash, KeyCode::LEFT);
	InputSystem::BindKey("RotateBlockRight"_hash, KeyCode::RIGHT);
	InputSystem::BindKey("PlaceAndRemove"_hash, KeyCode::SPACE);
	InputSystem::BindKey("Up"_hash, KeyCode::E);
	InputSystem::BindKey("Down"_hash, KeyCode::Q);
	InputSystem::BindKey("CameraLeft"_hash, KeyCode::MOUSE_LEFT);
	InputSystem::BindKey("CameraRight"_hash, KeyCode::MOUSE_RIGHT);
	InputSystem::BindVectorKeys("Move"_hash, KeyCode::W, KeyCode::S, KeyCode::A, KeyCode::D);
	InputSystem::BindVectorKeys("CameraMove"_hash, KeyCode::I, KeyCode::K, KeyCode::MOUSE_LEFT, KeyCode::MOUSE_RIGHT);

	InputSystem::BindPadButton("RotateBlockUp"_hash, PadCode::UP);
	InputSystem::BindPadButton("RotateBlockDown"_hash, PadCode::DOWN);
	InputSystem::BindPadButton("RotateBlockLeft"_hash, PadCode::LEFT);
	InputSystem::BindPadButton("RotateBlockRight"_hash, PadCode::RIGHT);
	InputSystem::BindPadButton("PlaceAndRemove"_hash, PadCode::B);
	InputSystem::BindPadButton("Up"_hash, PadCode::Y);
	InputSystem::BindPadButton("Down"_hash, PadCode::A);
	InputSystem::BindPadButton("CameraLeft"_hash, PadCode::LEFT_SHOULDER);
	InputSystem::BindPadButton("CameraRight"_hash, PadCode::RIGHT_SHOULDER);
	InputSystem::BindPadStick("Move"_hash, StickCode::LEFT);
	InputSystem::BindPadStick("CameraMove"_hash, StickCode::RIGHT);
}

void GameScene::CreateGridField()
{
	int size_x = 4;
	int size_y = 4;
	int size_z = 4;

	{
		//グリッドフィールドの作成
		auto obj = CreateGameObject();
		auto component = obj->AddComponent<GridField>();
		GameState::GetInstance()->SetGridField(component);
		component->SetSize({ size_x, size_y, size_z });
		ShapeType clearShapeX({ size_y, size_z });
		ShapeType clearShapeY({ size_x, size_z });
		ShapeType clearShapeZ({ size_x, size_y });
		clearShapeX.SetData({
			1, 0, 0, 0,
			1, 1, 0, 0,
			0, 1, 1, 0,
			0, 0, 0, 0,
			});
		clearShapeY.SetData({
			1, 1, 1, 0,
			1, 1, 1, 0,
			0, 0, 1, 1,
			0, 0, 0, 0,
			});
		clearShapeZ.SetData({
			1, 1, 1, 0,
			0, 0, 1, 0,
			0, 0, 1, 1,
			0, 0, 0, 0,
			});
		component->SetClearShape(clearShapeX, clearShapeY, clearShapeZ);

		obj->GetTransform()->SetPosition(0.0f, 0.2f, 0.0f);
	}

	// 投影機
	for (int i = 0; i < 2; ++i)
	{
		auto obj = CreateGameObject();
		auto renderer = obj->AddComponent<MeshRenderer>();
		renderer->LoadModel("Assets/Model/Stage/fbx/toueiki.fbx");

		Transform* transform = obj->GetTransform();
		if (i == 0)
		{
			transform->SetPosition(-(3.0f + size_x * 0.5f), 0.0f, 0.0f);
			transform->SetEulerAngle(0.0f, 90.0f, 0.0f);
		}
		else
		{
			transform->SetPosition(0.0f, 0.0f, 3.0f + size_z * 0.5f);
		}
	}

	// 危険エリア Z
	for (int i = 0; i < 2; ++i)
	{
		float flip = i == 0 ? 1.0f : -1.0f;
		for (int x = 0; x < size_x; ++x)
		{
			auto obj = CreateGameObject();
			auto renderer = obj->AddComponent<MeshRenderer>();
			renderer->LoadModel("Assets/Model/Stage/fbx/kiken.fbx");

			Vector3 pos;
			pos.x = x - size_x * 0.5f + 0.5f;
			pos.z = (size_z * 0.5f + 0.5f) * flip;
			obj->GetTransform()->SetPosition(pos);
			obj->GetTransform()->SetEulerAngle(0.0f, 90.0f * flip, 0.0f);
		}
	}

	// 危険エリア X
	for (int i = 0; i < 2; ++i)
	{
		float flip = i == 0 ? 1.0f : -1.0f;
		for (int z = 0; z < size_z; ++z)
		{
			auto obj = CreateGameObject();
			auto renderer = obj->AddComponent<MeshRenderer>();
			renderer->LoadModel("Assets/Model/Stage/fbx/kiken.fbx");

			Vector3 pos;
			pos.x = (size_x * 0.5f + 0.5f) * flip;
			pos.z = z - size_z * 0.5f + 0.5f;
			obj->GetTransform()->SetPosition(pos);
			obj->GetTransform()->SetEulerAngle(0.0f, 90.0f + 90.0f * flip, 0.0f);
		}
	}

	// 危険エリア コーナー
	for (int i = 0; i < 4; ++i)
	{
		Vector3 pos;
		pos.x = size_x * 0.5f + 0.5f;
		pos.z = size_z * 0.5f + 0.5f;
		if (i == 1 || i == 2)
		{
			pos.x *= -1;
		}
		if (i == 2 || i == 3)
		{
			pos.z *= -1;
		}

		auto obj = CreateGameObject();
		auto renderer = obj->AddComponent<MeshRenderer>();
		renderer->LoadModel("Assets/Model/Stage/fbx/Kiken_Corner.fbx");
		obj->GetTransform()->SetPosition(pos);
		obj->GetTransform()->SetEulerAngle(0.0f, 90.0f - 90.0f * i, 0.0f);
	}

	// 土台当たり判定用
	{
		// スケーリング時の判定がおかしい
		
		//auto obj = CreateGameObject();
		//obj->AddComponent<MeshRenderer>();
		//obj->GetTransform()->SetPosition(0.0f, 0.0f, 0.0f);
		//obj->GetTransform()->SetScale(size_x+1.0f, 0.2f, size_z);
		//auto collider = obj->AddComponent<Collider>();
		//collider->m_scale = Vector3(size_x + 2.0f, 0.01f, size_z + 2.0f);
	}
}

void GameScene::CreateStageSet()
{
	// ギミック以外のステージオブジェクト

	//床
	int stageSize_x = 24;
	int stageSize_z = 24;
	float blockScale = 1.0f;
	for (int z = 0; z < stageSize_z; ++z)
	{
		for (int x = 0; x < stageSize_x; ++x)
		{
			Vector3 pos(x - stageSize_x * 0.5f + 0.5f, 0.0f, z - stageSize_z * 0.5f + 0.5f);
			pos *= blockScale;

			auto obj = CreateGameObject();
			auto renderer = obj->AddComponent<MeshRenderer>();
			renderer->LoadModel("Assets/Model/Stage/fbx/yuka.fbx");
			obj->GetTransform()->SetPosition(pos);
			obj->GetTransform()->SetScale(0.25f, 0.25f, 0.25f);
		}
	}

	{
		//auto obj = CreateGameObject();
		//auto renderer = obj->AddComponent<MeshRenderer>();
		//renderer->LoadModel("Assets/Model/Stage/fbx/toueiki.fbx");
	}
}

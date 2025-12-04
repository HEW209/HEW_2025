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

	//移動できるオブジェクト（プレイヤー）を作成	
	//移動できるプレイヤーオブジェクトを作る
	auto player = CreateGameObject();
	player->AddComponent<Player>();

	//カメラ
	{
		auto Obj = CreateGameObject();
		auto playerCamera = Obj->AddComponent<PlayerCamera>();
		playerCamera->SetPlayer(player->GetTransform());
	}


	// シーンマネージャー
	{
		auto obj = CreateGameObject();
		obj->AddComponent<GameState>();
	}

	{//グリッドフィールドの作成

		auto obj = CreateGameObject();
		auto component = obj->AddComponent<GridField>();
		GameState::GetInstance()->SetGridField(component);
		component->SetSize({ 4, 4, 4 });
		ShapeType clearShapeX({ 4, 4 });
		ShapeType clearShapeY({ 4, 4 });
		ShapeType clearShapeZ({ 4, 4 });
		clearShapeX.SetData({
			1, 0, 0, 0,
			1, 1, 0, 0,
			0, 1, 1, 0,
			0, 0, 0, 0
			});
		clearShapeY.SetData({
			1, 1, 1, 0,
			1, 1, 1, 0,
			0, 0, 1, 1,
			0, 0, 0, 0
			});
		clearShapeZ.SetData({
			1, 1, 1, 0,
			0, 0, 1, 0,
			0, 0, 1, 1,
			0, 0, 0, 0
			});
		component->SetClearShape(clearShapeX, clearShapeY, clearShapeZ);

		obj->GetTransform()->SetPosition(0.0f, 0.0f, 0.0f);

	}

	//ワールドに配置するブロック
	{
		BlockSetData blockSet;
		blockSet.blocks.resize(1);
		blockSet.blocks[0] = { 0, 0, 0 };

		auto obj = CreateGameObject();
		auto component = obj->AddComponent<BlockObject>();
		component->SetBlockSet(blockSet);
		auto transform = obj->GetTransform();
		transform->SetPosition(Vector3{ 5.0f, 0.0f, 3.0f } + component->GetGroundOffset());

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
		transform->SetPosition(Vector3{ 5.0f, 0.0f, 1.0f } + component->GetGroundOffset());

		GameState::GetInstance()->AppendWorldBlock(component);
	}
	{
		BlockSetData blockSet;
		blockSet.blocks.resize(3);
		blockSet.blocks[0] = { 0, 0, 0 };
		blockSet.blocks[1] = { 1, 0, 0 };
		blockSet.blocks[2] = { 0, 1, 0 };

		auto obj = CreateGameObject();
		auto component = obj->AddComponent<BlockObject>();
		component->SetBlockSet(blockSet);
		auto transform = obj->GetTransform();
		transform->SetPosition(Vector3{ 5.0f, 0.0f, -1.0f } + component->GetGroundOffset());

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
		transform->SetPosition(Vector3{ 5.0f, 0.0f, -3.0f } + component->GetGroundOffset());

		GameState::GetInstance()->AppendWorldBlock(component);
	}

	{//ライト
		auto obj = CreateGameObject();
		obj->AddComponent<DirectionalLight>();
		obj->GetTransform()->SetEulerAngle(50.0f, -30.0f, 0.0f);
	}

}

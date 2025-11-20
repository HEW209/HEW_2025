#include "PlayerBlockDebugScene.h"

#include  "InputSystem.h"

//カメラ用
#include "SampleCamera.h"

//グリッド
#include "GameState.h"
#include "Player.h"

#include "PlayerBlockHandler.h"

void PlayerBlockDebugScene::Init()
{
	InputSystem::CreateButtonAction("RotateBlockUp");
	InputSystem::CreateButtonAction("RotateBlockDown");
	InputSystem::CreateButtonAction("RotateBlockLeft");
	InputSystem::CreateButtonAction("RotateBlockRight");
	InputSystem::CreateButtonAction("PlaceAndRemove");
	InputSystem::CreateButtonAction("Up");
	InputSystem::CreateButtonAction("Down");
	InputSystem::CreateButtonAction("CameraLeft");
	InputSystem::CreateButtonAction("CameraRight");
	InputSystem::CreateAxisAction("Move");

	InputSystem::BindKey("RotateBlockUp", KeyCode::UP);
	InputSystem::BindKey("RotateBlockDown", KeyCode::DOWN);
	InputSystem::BindKey("RotateBlockLeft", KeyCode::LEFT);
	InputSystem::BindKey("RotateBlockRight", KeyCode::RIGHT);
	InputSystem::BindKey("PlaceAndRemove", KeyCode::SPACE);
	InputSystem::BindKey("Up", KeyCode::E);
	InputSystem::BindKey("Down", KeyCode::Q);
	InputSystem::BindKey("CameraLeft", KeyCode::MOUSE_LEFT);
	InputSystem::BindKey("CameraRight", KeyCode::MOUSE_RIGHT);
	InputSystem::BindVectorKeys("Move", KeyCode::W, KeyCode::S, KeyCode::A, KeyCode::D);

	InputSystem::BindPadButton("RotateBlockUp", PadCode::UP);
	InputSystem::BindPadButton("RotateBlockDown", PadCode::DOWN);
	InputSystem::BindPadButton("RotateBlockLeft", PadCode::LEFT);
	InputSystem::BindPadButton("RotateBlockRight", PadCode::RIGHT);
	InputSystem::BindPadButton("PlaceAndRemove", PadCode::B);
	InputSystem::BindPadButton("Up", PadCode::Y);
	InputSystem::BindPadButton("Down", PadCode::A);
	InputSystem::BindPadButton("CameraLeft", PadCode::LEFT_SHOULDER);
	InputSystem::BindPadButton("CameraRight", PadCode::RIGHT_SHOULDER);
	InputSystem::BindPadStick("Move", StickCode::LEFT);
	
	//カメラ
	{
		auto rootObj = CreateGameObject();
		rootObj->AddComponent<SampleCamera>();
		rootObj->GetTransform()->SetEulerAngle(0.0f, 45.0f, 0.0f);

		auto obj = CreateGameObject();
		auto camera = obj->AddComponent<Camera>();
		Camera::Config cameraConfig;
		cameraConfig.fovAngle = 30.0f;
		camera->SetConfig(cameraConfig);
		auto transform = obj->GetTransform();
		transform->SetPosition(0.0f, 8.0f, -18.0f);
		transform->SetEulerAngle(20.0f, 0.0f, 0.0f);
		transform->SetParent(rootObj->GetTransform());
	}


	// シーンマネージャー
	{
		auto obj = CreateGameObject();
		obj->AddComponent<GameState>();
	}
	
	{
		//移動できるオブジェクト（プレイヤー）を作成	
		//移動できるプレイヤーオブジェクトを作る
		auto obj = CreateGameObject();
		obj->AddComponent<Player>();
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
		transform->SetPosition(Vector3{5.0f, 0.0f, 3.0f} + component->GetGroundOffset());
		
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



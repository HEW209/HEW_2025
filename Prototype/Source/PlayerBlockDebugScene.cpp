#include "PlayerBlockDebugScene.h"


//カメラ用
#include "SampleCamera.h"

//グリッド
#include "GameState.h"
#include "Player.h"

#include "PlayerBlockHandler.h"

void PlayerBlockDebugScene::Init()
{
	
	//カメラ
	{
		auto obj = CreateGameObject();
		auto camera = obj->AddComponent<Camera>();
		auto transform = obj->GetTransform();
		transform->SetEulerAngle(15.0f, 45.0f, 0.0f);
		obj->AddComponent<SampleCamera>();
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
	
	{//ライト
		auto obj = CreateGameObject();
		obj->AddComponent<DirectionalLight>();
		obj->GetTransform()->SetEulerAngle(50.0f, -30.0f, 0.0f);
	}
}



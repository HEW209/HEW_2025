#include "PlayerBlockDebugScene.h"


//カメラ用
#include "SampleCamera.h"

//グリッド
#include "GameState.h"

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
		auto playerObj = CreateGameObject();
		////プレイヤーにメッシュレンダラーを入れる。
		//auto renderer = playerObj->AddComponent<MeshRenderer>();
		////モデルを追加する。
		//renderer->LoadModel("Assets/Model/karubi/まけん式赤見かるび姫衣装ver1.0.pmx");
		 
		// パーツごとにキャラクターモデル追加 (オブジェクトを分けたら移動しちゃってOK)
		auto renderer1 = playerObj->AddComponent<MeshRenderer>();
		renderer1->LoadModel("Assets/Model/HEW/FBX/jyoubu.fbx");
		auto renderer2 = playerObj->AddComponent<MeshRenderer>();
		renderer2->LoadModel("Assets/Model/HEW/FBX/kabu.fbx");
		auto renderer3 = playerObj->AddComponent<MeshRenderer>();
		renderer3->LoadModel("Assets/Model/HEW/FBX/sityu.fbx");

		//プレイヤーオブジェクトにコンポーネントを追加
		playerObj->AddComponent<PlayerMove>();
		auto transform = playerObj->GetTransform();
		//大きさ
		transform->m_scale = { 1.0f, 1.0f, 1.0f };
		//ブロック操作コンポーネントの追加
		auto blockHandler = playerObj->AddComponent<PlayerBlockHandler>();

		//ブロックオブジェクトを生成
		auto blockObj= CreateGameObject();
		//オフセットで位置を変える
		blockObj->GetTransform()->SetPosition(0.0f , 2.0f , 0.0f);
		auto blockComponent = blockObj->AddComponent<BlockObject>();
		blockHandler->SetBlockObject(blockComponent);

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
			1, 0, 0, 0,
			0, 0, 0, 0,
			0, 0, 0, 0
			});
		clearShapeY.SetData({
			1, 1, 1, 0,
			1, 1, 1, 0,
			0, 0, 0, 0,
			0, 0, 0, 0
			});
		clearShapeZ.SetData({
			1, 1, 1, 0,
			0, 0, 0, 0,
			0, 0, 0, 0,
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



#include"StageSelectScene.h"
#include"StageNumber.h"
#include"StageSelectObject.h"
#include "BlockObject.h"
#include "StageInfo.h"
#include "Fade.h"
#include "InputManager.h"
#include "SoundManager.h"

void StageSelectScene::Init()
{
	SoundManager::PlayBGM("StageSelect", 1.0f, true);
	InputManager::ChangeBindType(InputBindType::UI);

	// カメラ
	{
		auto obj = CreateGameObject();
		auto camera = obj->AddComponent<Camera>();
		obj->GetTransform()->SetPosition(0.0f, 0.0f, -10.0f);
	}
	// ライト
	{
		auto lightObj = CreateGameObject();
		auto light = lightObj->AddComponent<DirectionalLight>();
		lightObj->GetTransform()->SetEulerAngle(50.0f, -30.0f, 0.0f);
	}
	// フェード
	{
		auto obj = CreateGameObject();
		auto fade = obj->AddComponent<Fade>();
		Fade::StartIrisIn();
	}

	// ステージ番号
	auto numberObj = CreateGameObject();
	auto stageNumber = numberObj->AddComponent<StageNumber>();
	// ステージオブジェクト
	{
		auto obj = CreateGameObject();
		auto selectObject = obj->AddComponent<StageSelectObject>();
		selectObject->SetStageNumber(stageNumber);
	}
	// コンベア枠
	{
		auto obj = CreateGameObject();

		auto mesh = obj->AddComponent<MeshRenderer>();
		mesh->LoadModel("Assets/Model/Stage/FBX/Conveyor_frame.fbx");
		obj->GetTransform()->SetPosition(0.0f, -6.3f, 0.0f);
		obj->GetTransform()->SetScale(1.5f, 1.0f, 2.1f);

	}

	// ステージ説明
	{
		auto obj = CreateGameObject();
		obj->GetTransform()->SetPosition(3.4f, 2.0f, 0.0f);
		auto info = obj->AddComponent<StageInfo>();
		info->SetStageNumber(stageNumber);
	}

	// その他
	{
		auto obj = CreateGameObject();
		auto renderer = obj->AddComponent<SpriteRenderer>();
		renderer->LoadTexture("Assets/Textures/StageSelect/StageSelect.png");
		renderer->SetSize(1280.0f);
		renderer->SetUI(true);
	}
	{
		auto obj = CreateGameObject();
		auto renderer = obj->AddComponent<SpriteRenderer>();
		renderer->LoadTexture("Assets/Textures/StageSelect/BackGround.png");
		renderer->SetSize(1280.0f);
		renderer->SetBackGround(true);
	}
}




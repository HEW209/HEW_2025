#include "StageNumber.h"
#include <memory>
#include "GameScene.h"
#include "InputManager.h"
#include "SaveData.h"
#include "Fade.h"
#include "GameState.h"
#include "TitleScene.h"
#include "StageSelectScene.h"

#define STAGE_FILE "Assets/Stage/Level%d.json"
#define FONT_SIZE (220.0f)

static const int digitIndex[10] =
{
0,  // 0の位置
1,  // 1
2,  // 2
3,  // 3
4,  // 4
5,  // 5
6,  // 6
7,  // 7
8,  // 8
9   // 9
};


StageNumber::StageNumber() :
	m_NumberSprite(),
	m_NumberSpriteBack(),
	m_selectIndex(1),
	m_lastInput(0),
	m_keyHold(false),
	m_holdTimer(0.0f),
	m_repeatTimer(0.0f),
	m_isSceneChange(false),
	m_targetScene(TargetScene::GAME)
{
}

void StageNumber::Start()
{
	float PosInterval = FONT_SIZE / 150.0f;	//文字間隔　＊　文字サイズによる間隔補正

	float PosX = -4.85f;
	float PosY = 0.9f;

	for (int x = 0; x < 3; ++x)
	{
		m_NumberSpriteBack[x] = GetGameObject()->AddComponent<SpriteRenderer>();
		m_NumberSpriteBack[x]->SetBackGround(true);
		m_NumberSpriteBack[x]->LoadTexture("Assets/Textures/Texts/Number.png");
		m_NumberSpriteBack[x]->SetOffsetPos(PosX, PosY);
		PosX += PosInterval;

		m_NumberSpriteBack[x]->SetUVScale(1.0f / 6.0f, 1.0f / 2.0f);
		m_NumberSpriteBack[x]->SetSize(FONT_SIZE, FONT_SIZE);
		m_NumberSpriteBack[x]->SetColor(0.9f, 0.5f, 0.0f, 1.0f);
	}

	PosX = -5.0f;
	PosY = 1.0f;

	for (int x = 0; x < 3; ++x)
	{
		m_NumberSprite[x] = GetGameObject()->AddComponent<SpriteRenderer>();
		m_NumberSprite[x]->SetBackGround(true);
		m_NumberSprite[x]->LoadTexture("Assets/Textures/Texts/Number.png");
		m_NumberSprite[x]->SetOffsetPos(PosX, PosY);
		PosX += PosInterval;

		m_NumberSprite[x]->SetUVScale(1.0f / 6.0f, 1.0f / 2.0f);
		m_NumberSprite[x]->SetSize(FONT_SIZE, FONT_SIZE);
		m_NumberSprite[x]->SetColor(0.9f, 0.9f, 0.9f, 1.0f);
	}

	m_selectIndex = SaveData::GetClearLevel() + 1;
	if (m_selectIndex > StageCount)
		m_selectIndex = StageCount;
}

void StageNumber::Update()
{
	if (m_isSceneChange)
	{
		if (!Fade::IsActive())
		{
			switch (m_targetScene)
			{
			case StageNumber::GAME:
				GameState::SetCurrentStegaNo(m_selectIndex);
				LoadGame();
				return;
				break;

			case StageNumber::TITLE:
				SceneManager::ChangeScene(std::make_unique<TitleScene>());
				break;
			}
		}
	}
	else
	{
		StageSelect();
		SetDigitUV();

		if (Input::GetKeyDown(KeyCode::ENTER) ||
			Input::GetButtonDown(PadCode::B))
		{
			m_isSceneChange = true;
			m_targetScene = TargetScene::GAME;
			Fade::StartIrisOut();
		}
		else if (Input::GetKeyDown(KeyCode::ESC) ||
			Input::GetButtonDown(PadCode::BACK))
		{
			m_isSceneChange = true;
			m_targetScene = TargetScene::TITLE;
			Fade::StartIrisOut();
		}
	}

#ifdef _DEBUG
	float deltaTime = Time::GetDeltaTime();
	int fps = 1.0f / deltaTime;

	ImGui::Begin("StageSelect");
	ImGui::Text("FPS : %3d", fps);

	int clearLevel = SaveData::GetClearLevel();
	ImGui::SliderInt("ClearLevel", &clearLevel, 0, StageCount);
	SaveData::SetClearLevel(clearLevel);

	if (ImGui::Button("Reload"))
	{
		SceneManager::ChangeScene(std::make_unique<StageSelectScene>());
	}

	ImGui::End();
#endif // DEBUG

#ifdef _DEBUG
	if (Input::GetKeyDown(KeyCode::KEY_0)) {
		SaveData::SetClearLevel(std::min(SaveData::GetClearLevel() + 1, StageCount));
	}
#endif // _DEBUG

}

int StageNumber::GetSelectIndex()
{
	return m_selectIndex;
}

std::string StageNumber::GetStageFilePath()
{
	std::string StageIDstr = std::to_string(m_selectIndex);
	std::string path = "Assets/Level/Stages/Level" + StageIDstr + ".json";
	return path;
}

void StageNumber::LoadGame()
{
	std::string StageIDstr = std::to_string(m_selectIndex);
	std::string path = "Level" + StageIDstr + "";

	InputManager::ChangeBindType(InputBindType::GAMEPLAY);
	SceneManager::ChangeScene(std::make_unique<GameScene>(path));//ステージ読み込むやつ

	//SceneManager::ChangeScene(std::make_unique<GameScene>("TestLevel01"));
}

void StageNumber::SetDigitUV()
{
	int number[3];
	int selectIndexTemp = m_selectIndex;
	for (int i = 2; i >= 0; --i)
	{
		number[i] = selectIndexTemp % 10;
		selectIndexTemp /= 10;
	}

	// 1マスのUVサイズ
	const float uSize = 1.0f / 6.0f;
	const float vSize = 1.0f / 2.0f;

	for (int x = 0; x < 3; ++x)
	{
		int index = number[x];

		float u = (index % 6) * uSize;
		float v = (index / 6) * vSize;

		m_NumberSprite[x]->SetUVOffsetPos(u, v);
		m_NumberSpriteBack[x]->SetUVOffsetPos(u, v);
	}
}

void StageNumber::StageSelect()
{
	// 入力取得
	int inputSide = 0;
	if (Input::GetLeftStick().x < 0.0f || Input::GetKeyDown(KeyCode::LEFT))
	{
		inputSide--;
	}
	if (Input::GetLeftStick().x > 0.0f || Input::GetKeyDown(KeyCode::RIGHT))
	{
		inputSide++;
	}

	// 選択移動
	if (m_lastInput == inputSide)
	{
		const float holdMoveWait = 0.7f;		// 長押し移動が始まるまでの時間
		const float holdMoveInterval = 0.1f;	// 長押し移動の時間間隔

		// 長押し処理
		m_holdTimer += Time::GetDeltaTime();
		if (m_holdTimer > holdMoveWait)
		{
			m_repeatTimer += Time::GetDeltaTime();
			if (m_repeatTimer > holdMoveInterval)
			{
				m_repeatTimer = 0.0f;
				m_selectIndex += inputSide;
			}
		}
	}
	else
	{
		// 押した瞬間の処理
		m_holdTimer = 0.0f;
		m_repeatTimer = 0.0f;
		m_selectIndex += inputSide;
	}
	m_lastInput = inputSide;

	// クランプ
	int maxStageNum = SaveData::GetClearLevel() + 1;
	if (maxStageNum > StageCount)
	{
		maxStageNum = StageCount;
	}
	if (m_selectIndex > maxStageNum)
	{
		m_selectIndex = maxStageNum;
	}
	if (m_selectIndex < 1)
	{
		m_selectIndex = 1;
	}
}
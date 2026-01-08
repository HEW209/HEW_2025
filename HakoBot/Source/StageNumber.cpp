#include "StageNumber.h"
#include <memory>
#include "GameScene.h"
#include "InputManager.h"
#include "SaveData.h"

#define STAGE_FILE "Assets/Stage/Level%d.json"
#define FONT_SIZE (200.0f)

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
	m_selectIndex(1),
	m_lastInput(0),
	m_keyHold(false),
	m_holdTimer(0.0f),
	m_repeatTimer(0.0f)
{
}

void StageNumber::Start()
{
	float PosX = -5.2f;
	float PosY = 1.0f;
	float PosInterval = FONT_SIZE / 100.0f;	//文字間隔　＊　文字サイズによる間隔補正

	for (int x = 0; x < 2; ++x)
	{
		m_NumberSprite[x] = GetGameObject()->AddComponent<SpriteRenderer>();
		m_NumberSprite[x]->SetUI(true);
		m_NumberSprite[x]->LoadTexture("Assets/Textures/Texts/Number.png");
		m_NumberSprite[x]->SetOffsetPos(PosX, PosY);
		PosX += PosInterval;

		m_NumberSprite[x]->SetUVScale(1.0f / 6.0f, 1.0f / 2.0f);
		m_NumberSprite[x]->SetSize(FONT_SIZE, FONT_SIZE);
	}
}

void StageNumber::Update()
{
	StageSelect();
	SetDigitUV();

	if (Input::GetKeyDown(KeyCode::ENTER) ||
		Input::GetButtonDown(PadCode::B))
	{
		LoadGame();
	}
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
	int number[2];
	number[0] = m_selectIndex / 10;
	number[1] = m_selectIndex % 10;

	// 1マスのUVサイズ
	const float uSize = 1.0f / 6.0f;
	const float vSize = 1.0f / 2.0f;

	for (int x = 0; x < 2; ++x)
	{
		int index = number[x];

		float u = (index % 6) * uSize;
		float v = (index / 6) * vSize;

		m_NumberSprite[x]->SetUVOffsetPos(u, v);
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
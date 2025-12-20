#include "StageNumber.h"
#include <memory>
#include "GameScene.h"
#include "InputManager.h"

#define STAGE_FILE "Assets/Stage/Level%d.json"
#define FONT_SIZE (450.0f)


int digitIndex[10] =
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


void StageNumber::Start()
{
	float PosX = -4.2f;
	float PosY = 1.3f;
	float PosInterval = 0.3f * (FONT_SIZE / 100);	//文字間隔　＊　文字サイズによる間隔補正

	for (int x = 0; x < 3; ++x)
	{
		m_NumberSprite[x] = GetGameObject()->AddComponent<SpriteRenderer>();
		m_NumberSprite[x]->SetUI(true);
		m_NumberSprite[x]->LoadTexture("Assets/Textures/newSprite.png");
		m_NumberSprite[x]->SetOffsetPos(PosX, PosY);
		PosX += PosInterval;

		m_NumberSprite[x]->SetUVScale(1.0f / 6.0f, 1.0f / 2.0f);
		m_NumberSprite[x]->SetSize(FONT_SIZE + 50.0f, FONT_SIZE + 50.0f);
	}

}

void StageNumber::Update()
{
	

	KeyEnter_Number();
		SetStegeNumberdigit();
		SetDigitUV();
}


void StageNumber::KeyEnter_Number()
{

	//============================================================================
	//						  ステージセレクトINDEX
	//============================================================================
	if (Input::GetKeyDown(KeyCode::UP))
	{
		m_bAutoCountUp = true;
		//m_selectIndex += 5;
		if (m_selectIndex >= m_stageCount) { m_selectIndex = 0; }
	}
	if (Input::GetKeyDown(KeyCode::DOWN))
	{
		m_bAutoCountDown = true;
		//m_selectIndex-=5;
		if (m_selectIndex < 0) { m_selectIndex = m_stageCount - 1; }
	}
	AutoCount();

	if (Input::GetKeyDown(KeyCode::LEFT))
	{
		m_selectIndex--;
		if (m_selectIndex < 0) { m_selectIndex = m_stageCount - 1; }

	}

	if (Input::GetKeyDown(KeyCode::RIGHT))
	{
		m_selectIndex++;
		if (m_selectIndex >= m_stageCount) { m_selectIndex = 0; }
	}
	//============================================================================




	if ((Input::GetKeyDown(KeyCode::ENTER)&&(m_bAutoCountUp!=true&& m_bAutoCountDown != true )))
	{
		LoadGame(m_selectIndex+1);
	}
}

void StageNumber::AutoCount()
{
	if (m_bAutoCountUp)
	{
		static int count = 0;
		if (m_totalTime >= 3)
		{
			m_totalTime = 0;
			m_selectIndex++;
			count++;
			if (m_selectIndex >= m_stageCount) { m_selectIndex = 0; }
		}
		else
		{
			m_totalTime++;
		}
		if (count > 4)
		{
			m_bAutoCountUp = false;
			count = 0;
		}


	}
	if (m_bAutoCountDown)
	{
		static int count = 0;
		if (m_totalTime >= 3)
		{
			m_totalTime = 0;
			m_selectIndex--;
			count++;
			if (m_selectIndex < 0) { m_selectIndex = m_stageCount - 1; }
		}
		else
		{
			m_totalTime++;
		}
		if (count > 4)
		{
			m_bAutoCountDown = false;
			count = 0;
		}
	}
}




void StageNumber::LoadGame(int StageID)
{
	char filePath[256];
	sprintf_s(filePath, STAGE_FILE, StageID);

	std::string StageIDstr = std::to_string(StageID);

	std::string path = "Level" + StageIDstr + "";

	InputManager::ChangeBindType(InputBindType::GAMEPLAY);
	SceneManager::ChangeScene(std::make_unique<GameScene>(path));//ステージ読み込むやつ


	//SceneManager::ChangeScene(std::make_unique<GameScene>("TestLevel01"));



}


void StageNumber::SetDigitUV()
{
	// 1マスのUVサイズ
	const float uSize = 1.0f / 6.0f;
	const float vSize = 1.0f / 2.0f;

	for (int x = 0; x < 3; ++x)
	{
		int index = digitIndex[digit[x]];

		float u = (index % 6) * uSize;
		float v = (index / 6) * vSize;

		m_NumberSprite[x]->SetUVOffsetPos(u, v);
	}
}

void StageNumber::SetStegeNumberdigit()
{

	int totalSeconds = m_selectIndex+1;

	int Hundred = totalSeconds / 100;
	int Ten = (totalSeconds / 10);
	int One = totalSeconds % 10;

	digit[0] = Hundred ;
	digit[1] = Ten;//1が一桁目、2が二桁目
	digit[2] = One;
}

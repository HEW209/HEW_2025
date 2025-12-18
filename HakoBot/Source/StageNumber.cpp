#include "StageNumber.h"

#define STAGE_FILE "Asets/Stage/StageDate%d.json"

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
	float PosInterval = 0.3f * (MOZI_SIZE / 100);	//文字間隔　＊　文字サイズによる間隔補正

	for (int x = 0; x < 3; ++x)
	{
		sprite[x] = GetGameObject()->AddComponent<SpriteRenderer>();
		sprite[x]->SetUI(true);
		sprite[x]->LoadTexture("Assets/Textures/newSprite.png");
		sprite[x]->SetOffsetPos(PosX, PosY);
		PosX += PosInterval;

		sprite[x]->SetUVScale(1.0f / 6.0f, 1.0f / 2.0f);
		sprite[x]->SetSize(MOZI_SIZE + 50.0f, MOZI_SIZE + 50.0f);
	}

}

void StageNumber::Update()
{
	

		KeyEnter();
		SetStegeNumberdigit();
		SetDigitUV();
}


void StageNumber::KeyEnter()
{

	//============================================================================
	//						  ステージセレクトINDEX
	//============================================================================
	if (Input::GetKeyUp(KeyCode::UP))
	{
		m_bAutoCountUp = true;
		//m_selectIndex += 5;
		if (m_selectIndex >= m_stageCount) { m_selectIndex = 0; }
	}
	if (Input::GetKeyUp(KeyCode::DOWN))
	{
		m_bAutoCountDown = true;
		//m_selectIndex-=5;
		if (m_selectIndex < 0) { m_selectIndex = m_stageCount - 1; }
	}
	AutoCount();

	if (Input::GetKeyUp(KeyCode::LEFT))
	{
		m_selectIndex--;
		if (m_selectIndex < 0) { m_selectIndex = m_stageCount - 1; }

	}

	if (Input::GetKeyUp(KeyCode::RIGHT))
	{
		m_selectIndex++;
		if (m_selectIndex >= m_stageCount) { m_selectIndex = 0; }
	}
	//============================================================================




	if (Input::GetKeyUp(KeyCode::ENTER))
	{
		int StageID;								//int型ステージの代償を結合する変数
		StageID = m_selectIndex;			//1の桁を代入
		if (StageID == 0)							//0だった場合：メニューに戻る処理、
		{											//これは1-0・2-0・3-0全てはメニューに戻るEx
			//Exit メニューに移行の処理
		}
		else
		{											//1の桁が0ではなかった場合、2桁目にステージの面（大）を代入（ステージ2-5の場合・25になる）
			LoadGame(StageID);
		}
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
	
	if (StageID == 0)
	{
		//Exit処理				//タイトルに戻るやつ
	}
	else
	{
		//SceneManager::ChangeScene(std::make_unique<GameScene>(filePath));//ステージ読み込むやつ
			
	}
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

		sprite[x]->SetUVOffsetPos(u, v);
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

#include "StageNumber.h"

#define STAGE_FILE "Asets/Stage/StageDate%d.json"
void StageNumber::Start()
{

}

void StageNumber::Update()
{
		KeyEnter();
}


void StageNumber::KeyEnter()
{

	//============================================================================
	//						  ステージセレクトINDEX
	//============================================================================
	if (Input::GetKeyUp(KeyCode::UP))
	{
		m_selectIndex -=5;
		if (m_selectIndex < 0) { m_selectIndex = m_stageCount - 1; }
	}

	if (Input::GetKeyUp(KeyCode::DOWN))
	{
		m_selectIndex +=5;
		if (m_selectIndex >= m_stageCount) { m_selectIndex = 0; }
	}
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

void StageNumber::Set_StegeNumberdigit(int CallNum)
{
	m_StegeNumberdigit = CallNum;
}

Vector2 StageNumber::SetPos_StegeNumber(int CallNum)
{
	float StageDrawPosX = -4.4f;			//100の桁の位置X
	float StageDrawPosY = 1.5f;			//100の桁の位置X
	float StageDrawinterval = 1.5f;		//ステージ番号の表示間隔

	Vector2 ReturnPos;

	ReturnPos.x = (StageDrawPosX + (CallNum * StageDrawinterval));
	ReturnPos.y = StageDrawPosY;

	return ReturnPos;
}

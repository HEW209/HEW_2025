#include "StageSelectObject.h"
#include"StageSelectKeyEnter.h"
#define STAGE_FILE "Asets/Stage/StageDate%d.json"
void StageSelectObject::Start()
{

}

void StageSelectObject::Update()
{
	if (m_StageID == 11)
	{
		Vector2 SetPos = SetPosID(m_UP_DOWN_selectIndex);
	KeyEnter();
	GetTransform()->SetPosition(SetPos.x, SetPos.y,0.0f);
	
	}


}


void StageSelectObject::KeyEnter()
{

	//============================================================================
	//						  ステージセレクトINDEX
	//============================================================================
	if (Input::GetKeyUp(KeyCode::UP))
	{
		m_UP_DOWN_selectIndex--;
		if (m_UP_DOWN_selectIndex < 0) { m_UP_DOWN_selectIndex = m_SMALL_stageCount; }

	}

	if (Input::GetKeyUp(KeyCode::DOWN))
	{
		m_UP_DOWN_selectIndex++;
		if (m_UP_DOWN_selectIndex > m_SMALL_stageCount) { m_UP_DOWN_selectIndex = 0; }	//ステージ数は１０だが終了用に1増やしてある
	}
	if (Input::GetKeyUp(KeyCode::LEFT))
	{
		m_LEFT_RIGHT_selectIndex--;
		if (m_LEFT_RIGHT_selectIndex < 0) { m_LEFT_RIGHT_selectIndex = m_BIG_stageCount - 1; }

	}

	if (Input::GetKeyUp(KeyCode::RIGHT))
	{
		m_LEFT_RIGHT_selectIndex++;
		if (m_LEFT_RIGHT_selectIndex >= m_BIG_stageCount) { m_LEFT_RIGHT_selectIndex = 0; }
	}
	//============================================================================




	if (Input::GetKeyUp(KeyCode::ENTER))
	{
		int StageID;								//int型ステージの代償を結合する変数
		StageID = m_UP_DOWN_selectIndex;			//1の桁を代入
		if (StageID == 0)							//0だった場合：メニューに戻る処理、
		{											//これは1-0・2-0・3-0全てはメニューに戻るEx
			//Exit メニューに移行の処理
		}
		else
		{											//1の桁が0ではなかった場合、2桁目にステージの面（大）を代入（ステージ2-5の場合・25になる）
			StageID = m_UP_DOWN_selectIndex + (m_BIG_stageCount * 100);
			LoadGame(StageID);
		}
	}
}

void StageSelectObject::SetStageID(int StageID)
{
	m_StageID = StageID;
}




Vector2 StageSelectObject::SetPosID(int StageID)
{
	float StageDrawPosX = 2.4f;
	float StageDrawPosY = 1.4f;

	
	int ones = StageID;
	Vector2 ReturnPos;

	if (ones == 0)
	{
		ReturnPos.x = -5.8f;
		ReturnPos.y= 3.0f;
		return ReturnPos;
	}
	else
	{
		switch (ones)
		{
		case 1:
			ReturnPos.x = (StageDrawPosX * -2.0f);
			ReturnPos.y = StageDrawPosY;
			return ReturnPos;
			break;
		case 2:
			ReturnPos.x = (StageDrawPosX * -1.0f);
			ReturnPos.y = StageDrawPosY;
			return ReturnPos;
		case 3:
			ReturnPos.x = 0.0f;
			ReturnPos.y = StageDrawPosY;
			return ReturnPos;
		case 4:
			ReturnPos.x = (StageDrawPosX * 1.0f);
			ReturnPos.y = StageDrawPosY;
			return ReturnPos;
		case 5:
			ReturnPos.x = (StageDrawPosX * 2.0f);
			ReturnPos.y = StageDrawPosY;
			return ReturnPos;
		case 6:
			ReturnPos.x = (StageDrawPosX * -2.0f);
			ReturnPos.y = (StageDrawPosY*-1.0f);
			return ReturnPos;
		case 7:
			ReturnPos.x = (StageDrawPosX * -1.0f);
			ReturnPos.y = (StageDrawPosY * -1.0f);
			return ReturnPos;
		case 8:
			ReturnPos.x = 0.0f;
			ReturnPos.y = (StageDrawPosY * -1.0f);
			return ReturnPos;
		case 9:
			ReturnPos.x = (StageDrawPosX);
			ReturnPos.y = (StageDrawPosY * -1.0f);
			return ReturnPos;				
		case 10:
			ReturnPos.x = (StageDrawPosX * 2.0f);
			ReturnPos.y = (StageDrawPosY * -1.0f);
			return ReturnPos;
			break;
		case 11:
			ReturnPos.x = 0.0f;
			ReturnPos.y = 0.0f;
			break;
		default:
			break;
		}
	}
	
}



void StageSelectObject::LoadGame(int StageID)
{
	char filePath[256];
	sprintf_s(filePath, STAGE_FILE, StageID);
	int ones = m_StageID % 100;
	if (ones == 0)
	{
		//Exit処理				//タイトルに戻るやつ
	}
	else
	{
		//LoadGame(filePath)	//ステージ読み込むやつ
	}
}






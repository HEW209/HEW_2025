#include "StageSelectObject.h"

#define STAGE_FILE "Asets/Stage/StageDate%d.json"
void StageSelectObject::Start()
{

}

void StageSelectObject::Update()
{
		KeyEnter();
		Vector2 SetPos = SetPosID(m_selectIndex);
		GetTransform()->SetPosition(SetPos.x, SetPos.y, 0.0f);
}


void StageSelectObject::KeyEnter()
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

void StageSelectObject::SetStageID(int StageID)
{
	m_StageID = StageID;
}




Vector2 StageSelectObject::SetPosID(int StageID)
{
	float StageDrawPosX = 0.0f;
	float StageDrawPosY = 0.0f;

	
	Vector2 ReturnPos;

	ReturnPos.x = (StageDrawPosX + ((StageID-m_selectIndex) *5.0f));
	ReturnPos.y = StageDrawPosY;
	
	return ReturnPos;
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
		//SceneManager::ChangeScene(std::make_unique<GameScene>(filePath));//ステージ読み込むやつ
			
	}
}






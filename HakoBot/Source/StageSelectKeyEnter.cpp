#include "StageSelectKeyEnter.h"

void StegeSelectKeyEnter::Start()
{
	m_SelectObject->GetGameObject()->GetComponent<StageSelectObject>();
}

void StegeSelectKeyEnter::Update()
{
	//============================================================================
	//						  ステージセレクトINDEX
	//============================================================================
	if (Input::GetKeyUp(KeyCode::UP))
	{
		m_UP_DOWN_selectIndex--;
		if (m_UP_DOWN_selectIndex < 0) { m_UP_DOWN_selectIndex = m_SMALL_stageCount - 1; }

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
			StageID = m_UP_DOWN_selectIndex + (m_BIG_stageCount * 10);
			m_SelectObject->LoadGame(StageID);
		}
	}
}

int StegeSelectKeyEnter::GetMenuID()
{
	m_MenuID++;
	return m_MenuID - 1;
}



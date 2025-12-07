#include "StageSelectManager.h"

void StageSelectManager::Start()
{
	a->GetGameObject()->GetComponent<StageSelectManager>();
}

void StageSelectManager::Update()
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
}

int StageSelectManager::SetMenuID()
{
	m_SMALL_stageCount;
	return m_MenuID-1;
}






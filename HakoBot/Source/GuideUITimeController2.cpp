//GuideUITimeController2.cpp
#include "GuideUITimeController2.h"
#include "GameState.h"
#include "InputManager.h"


void GuideUITimeController2::Start()
{
	m_b3 = false;
}

void GuideUITimeController2::Update()
{
	GridField* gridfield = GameState::GetInstance()->GetGridField();

	if (gridfield->IsClear()&& InputManager::CurrentInputSystem().GetButtonDown("Clear"_hash))
	{
		m_b3 = true;
		
	}

	if (m_b3)
	{
		m_time2++;
	}

	if (m_time2 / 60 > 4)
	{
		GetTransform()->SetScale(0.2f, 0.2f, 0.0f);
	}
}

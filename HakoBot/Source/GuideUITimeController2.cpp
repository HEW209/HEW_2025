//GuideUITimeController2.cpp
#include "GuideUITimeController2.h"
#include "GameState.h"
#include "InputSystem.h"


void GuideUITimeController2::Start()
{
	
}

void GuideUITimeController2::Update()
{
	GridField* gridfield = GameState::GetInstance()->GetGridField();

	if (gridfield->IsClear()&& InputSystem::GetButtonDown("Clear"_hash))
	{
		GetTransform()->SetScale(0.2f, 0.2f, 0.0f);
	}
}

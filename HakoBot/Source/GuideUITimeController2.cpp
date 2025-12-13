//GuideUITimeController2.cpp
#include "GuideUITimeController2.h"
#include "GameState.h"


void GuideUITimeController2::Start()
{
	
}

void GuideUITimeController2::Update()
{
	GridField* gridfield = GameState::GetInstance()->GetGridField();

	if (gridfield->IsClear()&& Input::GetKeyDown(KeyCode::X))
	{
		GetTransform()->SetScale(0.2f, 0.2f, 0.0f);
	}
}

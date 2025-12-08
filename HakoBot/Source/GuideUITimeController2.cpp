//GuideUITimeController2.cpp
#include "GuideUITimeController2.h"


void GuideUITimeController2::Start()
{
	
}

void GuideUITimeController2::Update()
{
	if (Input::GetKeyDown(KeyCode::A))
	{
		GetTransform()->SetScale(0.2f, 0.2f, 0.0f);
	}
}

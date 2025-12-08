#include "StageSelectObject.h"
#include"StageSelectKeyEnter.h"
void StageSelectObject::Start()
{
	StageKeyEnter->GetGameObject()->GetComponent<StegeSelectKeyEnter>();
	m_StageID=StageKeyEnter->GetMenuID();
}

void StageSelectObject::Update()
{
	

}


void StageSelectObject::LoadGame(int StageID)
{
	
}


Vector2 StageSelectObject::SetPosID(int StageID)
{
	
	int ones = StageID % 10;
	
	Vector2 ReturnPos;

	if (ones == 0)
	{
		ReturnPos.x = -5.8f;
		ReturnPos.y= 3.0f;
		return ReturnPos;
	}
	else
	{
		
	}
	
}









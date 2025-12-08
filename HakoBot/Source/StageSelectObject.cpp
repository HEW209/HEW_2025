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
	
	int ones = StageID % 100;
	
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
			ReturnPos.x = -4.8f;
			ReturnPos.y = 1.8f;
			return ReturnPos;
			break;
		case 2:
			ReturnPos.x = -2.4f;
			ReturnPos.y = 1.8f;
			return ReturnPos;
		case 3:
			ReturnPos.x = 0.0f;
			ReturnPos.y = 1.8f;
			return ReturnPos;
		case 4:
			ReturnPos.x = 2.4f;	
			ReturnPos.y = 1.8f;
			return ReturnPos;
		case 5:
			ReturnPos.x = 4.8f;	
			ReturnPos.y = 1.8f;
			return ReturnPos;
		case 6:
			ReturnPos.x = -4.8f;
			ReturnPos.y = -1.8f;
			return ReturnPos;
		case 7:
			ReturnPos.x = -2.4f;
			ReturnPos.y = -1.8f;
			return ReturnPos;
		case 8:
			ReturnPos.x = 0.0f;
			ReturnPos.y = -1.8f;
			return ReturnPos;
		case 9:
			ReturnPos.x = 2.4f;
			ReturnPos.y = -1.8f;
			return ReturnPos;				
		case 10:
			ReturnPos.x = 4.8f;
			ReturnPos.y = -1.8f;
			return ReturnPos;
			break;
		default:
			break;
		}
	}
	
}









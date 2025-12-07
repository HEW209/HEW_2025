#include "StageSelectObject.h"

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
	//Stage(StageID);
}







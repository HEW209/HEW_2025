#include"SampleStageSelectScene.h"
#include"StageNumber.h"
#include"StageSelectObject.h"


void StageSelectScene::Init()
{
	{
		auto obj = CreateGameObject();
		auto camera = obj->AddComponent<Camera>();
	}
	
	/*
	for(int i =0;i<=31;i++)
	{
		{
			auto obj = CreateGameObject();
		
			auto SelectObject = obj->AddComponent<StageSelectObject>();
			
			auto rendere = obj->AddComponent<SpriteRenderer>();
			rendere->SetUI(true);

			
			SelectObject->SetStageID(i);
			
			Vector2 SetPos = SelectObject->SetPosID(i);
			rendere->SetOffsetPos(SetPos);
		}
	}
	*/
	
	for (int i = 0; i < 3; i++)
	{
		{
			auto obj = CreateGameObject();
			auto SelectNumber = obj->AddComponent<StageNumber>();

			auto rendere = obj->AddComponent<SpriteRenderer>();
			rendere->SetUI(true);


			SelectNumber->Set_StegeNumberdigit(i);

			Vector2 SetPos = SelectNumber->SetPos_StegeNumber(i);

			rendere->SetOffsetPos(SetPos);
		}
	}
}




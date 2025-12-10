#include"SampleStageSelectScene.h"
#include"StageSelectObject.h"
#include"StageSelectKeyEnter.h"


void StageSelectScene::Init()
{
	{
		auto obj = CreateGameObject();
		auto camera = obj->AddComponent<Camera>();
	}
	

	for(int i =0;i<=11;i++)
	{
		{
			auto obj = CreateGameObject();
		
			auto SelectObject = obj->AddComponent<StageSelectObject>();
			
			auto rendere = obj->AddComponent<SpriteRenderer>();
			rendere->SetUI(true);
			if (i == 11)
			{
				rendere->LoadTexture("Assets/Textures/Cyan.png");
				rendere->SetSize(50);
			}
			SelectObject->SetStageID(i);
			
			Vector2 SetPos = SelectObject->SetPosID(i);
			rendere->SetOffsetPos(SetPos);
		}
	}
	
	
}




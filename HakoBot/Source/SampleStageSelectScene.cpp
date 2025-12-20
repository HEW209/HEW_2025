#include"SampleStageSelectScene.h"
#include"StageNumber.h"
#include"StageSelectObject.h"
#include "BlockObject.h"

#include "LevelSerializer.h"

void StageSelectScene::Init()
{
	{
		auto obj = CreateGameObject();
		auto camera = obj->AddComponent<Camera>();
		obj->GetTransform()->SetPosition(0.0f, 0.0f, -10.0f);
	}
	
	

		
		for (int i = 0; i < 30; i++)
		{
			std::string blockName = "tate3masu";
			std::string blockPath = "Assets/Model/Blocks/FBX/" + blockName + ".fbx";

			auto obj = CreateGameObject();
			auto SelectObject = obj->AddComponent<StageSelectObject>();
			SelectObject->SetStageID(i);
			
			auto mesh = obj->AddComponent<MeshRenderer>();
			mesh->LoadModel(blockPath);
			
			obj->GetTransform()->SetPosition(20.0f,-2.0f,0.0f);
		}

		
				
	

	
	{
		auto lightObj = CreateGameObject();
		auto light = lightObj->AddComponent<DirectionalLight>();
		lightObj->GetTransform()->SetEulerAngle(50.0f, -30.0f, 0.0f);
	}
    
    

	

	{
		auto obj = CreateGameObject();
		auto SelectNumber = obj->AddComponent<StageNumber>();
	}
	

}




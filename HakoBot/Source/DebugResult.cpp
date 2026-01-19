#include "DebugResult.h"
#include "InputManager.h"
#include "GameScene.h"
#include "ResultController.h"
//DebugResult.cpp

void DebugResult::Start()
{

}

void DebugResult::Update()
{
	//Pキーでリザルト表示
	if (InputManager::CurrentInputSystem().GetButtonDown("Result"_hash))
	{
		
		CreateResult();
	}
}

void DebugResult::CreateResult()
{
	GetGameObject()->AddComponent<ResultController>();
}
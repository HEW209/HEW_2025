#include "TitleExit.h"
#include <memory>
#include "GameScene.h"

void TitleExit::Update()
{
	if (Input::GetKeyDown(KeyCode::ENTER) || Input::GetButtonDown(PadCode::B))
	{
		SceneManager::ChangeScene(std::make_unique<GameScene>("TestLevel01"));
	}
}

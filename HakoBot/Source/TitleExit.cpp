#include "TitleExit.h"
#include <memory>
#include "GameScene.h"
#include "InputManager.h"

void TitleExit::Update()
{
	if (InputManager::CurrentInputSystem().GetButtonDown("MenuInteract"_hash))
	{
		InputManager::ChangeBindType(InputBindType::GAMEPLAY);
		SceneManager::ChangeScene(std::make_unique<GameScene>("TestLevel01"));
	}
}

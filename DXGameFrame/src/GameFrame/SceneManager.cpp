//SceneManager.cpp
#include <GameFrame/SceneManager.h>

std::unique_ptr<Scene> SceneManager::s_activeScene = nullptr;
std::unique_ptr<Scene> SceneManager::s_nextScene = nullptr;

void SceneManager::Init(std::unique_ptr<Scene> startScene)
{
	s_activeScene = std::move(startScene);
	s_activeScene->Init();
}

void SceneManager::Uninit()
{
	s_activeScene->Uninit();
	s_activeScene = nullptr;
	s_nextScene = nullptr;
}

void SceneManager::Execute()
{
	if (s_activeScene == nullptr)
		return;

	s_activeScene->Update();
	ApplyChangeScene();
}

void SceneManager::ChangeScene(std::unique_ptr<Scene> nextScene)
{
	if (s_nextScene == nullptr)
		s_nextScene = std::move(nextScene);
}

void SceneManager::ApplyChangeScene()
{
	if (s_nextScene == nullptr)
		return;

	if (s_activeScene != nullptr)
	{
		s_activeScene->Uninit();
	}

	s_activeScene = std::move(s_nextScene);
	s_nextScene = nullptr;
	s_activeScene->Init();
}
